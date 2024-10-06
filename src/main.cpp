
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>  // for usleep
#include <chrono>
#include <cstdio>
#include <cstdlib>  // for system()
#include <fstream>
#include <iostream>
#include <thread>
#include "GameConfig.hpp"

void display_story(GameConfig& config, int rows, int cols) {
	(void)cols;
    std::string line;
    int i = 0;
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
            usleep(50000);
        }
    }
}

void init_game() {
    (void)initscr();
    (void)cbreak();
    (void)noecho();
    start_color();                          // allow colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // color pair 1
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
	for (int y = 0; y < LINES; ++y) {
        for (int x = 0; x < COLS * 2; ++x) {
			uint8_t ch = rand() % 128;
            mvwaddch(my_pad, y, x, ch); // 
        }
    }

	int offset = 0;
    while ((c = getch()) != 27) {
        if (!config.storyPlayed) {
			nodelay(stdscr, FALSE); 
            display_story(config, LINES, COLS);
			mvwprintw(stdscr, 0, LINES - 10, "%s", "hello");
            if ((c) != 27 && (c) != ERR) {
                config.storyPlayed = true;
				nodelay(stdscr, TRUE); 
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

			usleep(50000);

			prefresh(my_pad, 0, offset, 0, 0, LINES - 1, COLS - 1);

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
    endwin();
    return (0);
}
