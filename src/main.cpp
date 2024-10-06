
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>  // for usleep
#include <chrono>
#include <cstdio>
#include <cstdlib>  // for system()
#include <fstream>
#include <iostream>
#include <thread>
#include <cwchar>
#include "GameConfig.hpp"

void display_story(GameConfig& config, int rows, int cols) {
	(void)cols;
    std::string line;
    int i = 0;
	// wchar_t *str = L"Hello, world! こんにちは世界！";
	// 任意のボタンを押してください
    // Print the wide character string
    // mvwaddwstr(stdscr, 5, 5, str);
	// refresh();
    if ((int)config.intro.size() == 0) {
        mvprintw(0, 0, "...loading story...");
    }
    if ((int)config.intro.size() > rows) {
        mvprintw(i + 1, 15, "%s", "Press Any Keys To Continue");
        refresh();
    } else {
        for (const std::string& line : config.intro) {
            mvprintw(i + 1, 15, "%s", line.c_str());
            i++;
            refresh();
            usleep(10000);
        }
    }
}

void init_game() {
    (void)initscr();
    (void)cbreak();
    (void)noecho();
	setlocale(LC_ALL, "");
    start_color();                          // allow colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // color pair 1
	init_pair(2, COLOR_RED, COLOR_BLACK);   // color pair 2
	init_pair(3, COLOR_GREEN, COLOR_BLACK); // color pair 3
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);// color pair 4
	init_pair(5, COLOR_BLUE, COLOR_BLACK);  // color pair 5
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);// color pair 6
	init_pair(7, COLOR_WHITE, COLOR_BLACK); // color pair 7
    keypad(stdscr, TRUE);                   // enable function and cursor keys
    (void)nonl();  // do not add newline when pressing enter
    cbreak();      // do not buffer input
    noecho();      // do not echo input
    curs_set(0);   // do not show cursor
}

typedef struct {
	double fps;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
	std::chrono::duration<double> elapsedTime;	
	std::chrono::duration<double> remainingTime;
}	GameParams;


// typedef enum {
// 	BLOCK = 0,
// 	EMPTY,
// }	BlockType;

void fillBackground(WINDOW *my_pad)
{
	for (int y = 1; y < LINES; ++y) {
        for (int x = 0; x < COLS * 2; ++x) {
			int colorPair = 1 + rand() % 3;
			wattrset(my_pad, COLOR_PAIR(colorPair));
            mvwaddwstr(my_pad, y, x, L"█"); 
			wattroff(my_pad, COLOR_PAIR(colorPair));
        }
    }
	// for (int y = 0; y < LINES; ++y) {
    //     for (int x = 0; x < COLS * 2; ++x) {
	// 		uint8_t ch = rand() % 128;
    //         mvwaddch(my_pad, y, x, ch); // 
    //     }
    // }
} 

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;

    usleep(100000); // give time to alacritty to set up the terminal
    GameConfig& config = GameConfig::getConf();
	GameParams params;

    init_game();
 
	nodelay(stdscr, TRUE); // non-blocking getch

    params.fps = 60.0;
    params.startTime = std::chrono::high_resolution_clock::now();
    // auto lastTime = startTime;
    int c = 0;
	WINDOW* my_pad = newpad(LINES, 300);
	
	if (my_pad == nullptr) {
        endwin();
        fprintf(stderr, "Error creating pad\n");
        return 1;
    }

	fillBackground(my_pad); 

	int offset = 0;
    while ((c = getch()) != 27) {
        if (!config.storyPlayed) {
			nodelay(stdscr, FALSE); 
            display_story(config, LINES, COLS);
            if ((c) != 27 && (c) != ERR) {
                config.storyPlayed = true;
				nodelay(stdscr, TRUE); 
				werase(stdscr);
                continue;
            } else if ((c) != ERR) {
				break;
			}
       
        } else {
			if (c == 27) {
				break;
			}
            // werase(stdscr);
            // pnoutrefresh(my_pad, 0, offset, 0, 0, LINES - 1, COLS - 1);

			usleep(200000);

			prefresh(my_pad, 0, offset, 0, 0, 9, COLS - 1);
			prefresh(my_pad, LINES - 10, offset, LINES - 10, 0, LINES - 1, COLS - 1);

			char fpsStr[10];
            snprintf(fpsStr, sizeof(fpsStr), "fps %.2f", params.fps);
            mvwprintw(stdscr, 0, LINES - 10, "%s", fpsStr);

            attron(COLOR_PAIR(1));
            mvwprintw(stdscr, 0, 0, "Pressed: %d", c);
            attroff(COLOR_PAIR(1));
			refresh();

			offset++;
			if (offset >= COLS) {
				offset = 0;
			}

            params.endTime = std::chrono::high_resolution_clock::now();
            params.elapsedTime = params.endTime - params.startTime;
            params.remainingTime = std::chrono::duration<double>(1.0 / 60.0) - params.elapsedTime;
            if (params.remainingTime > std::chrono::duration<double>(0.0)) {
                std::this_thread::sleep_for(params.remainingTime);
            } else {
                mvwprintw(stdscr, 0, COLS - 10, "fps ...");
            }
        }
    }
	delwin(my_pad);
    endwin();
    return (0);
}
