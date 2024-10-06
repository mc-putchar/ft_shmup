
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
#include <algorithm>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <curses.h>

#include "Enemy.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "ft_shmup.hpp"

namespace ft_shmup {
static volatile std::sig_atomic_t g_stop;
void stop_game(int sig) {
    (void)sig;
    g_stop = 1;
    cleanup_and_exit();
}
}  // namespace ft_shmup

void create_enemies(std::vector<Enemy>& enemies, int n) {
    // ,_,
    // |!|
    // ```
    Texture bullet_tex(1, 1, "-");
    Texture laser_icon(3, 3, ",_,|!|```");
    Weapon laser(laser_icon, 1000, 1, bullet_tex);
    Texture test_tex(6, 1, " (0){~");
    int16_t x(140);
    Point dir(-1, 0);
    for (int i = 0; i < n; ++i) {
        Point p(x, 20);
        Enemy enemy(p, 3, 0, test_tex);
        enemy.set_direction(dir);
        Pisciner& pisciner = static_cast<Pisciner&>(enemy);
        pisciner.set_weapon(&laser);
        enemies.push_back(enemy);
        x += 10;
    }
}

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;
    std::signal(SIGINT, ft_shmup::stop_game);
    std::signal(SIGTERM, ft_shmup::stop_game);
    Game game;
    init_screen(game);

    Texture skin(10, 3, "~>L-\\___  ~XE[]==O}>~>F-/```  ");
    Player p(Point(20, 20), 10, 10, skin);
    p.move(stdscr, Point(0, 0));  // Draw player at start position

    std::vector<Enemy> enemies;
    create_enemies(enemies, 5);

    // Idea for control:
    // - Detect key down and key up events
    //    - If key down, toggle the flag for that key
    //    - If key up, toggle the flag back to false fot that key
    // - In the main loop, check the flags and move the player accordingly

    // Primitive control loop
    int c;
    while (!ft_shmup::g_stop) {
        c = wgetch(stdscr);
        wclear(stdscr);
        switch (c) {
            case KEY_ESC:
                cleanup_and_exit();
                break;
            case KEY_UP:
                mvwprintw(stdscr, 10, 40, "KEY_UP");
                p.move(stdscr, Point(0, -1));
                break;
            case KEY_DOWN:
                mvwprintw(stdscr, 10, 40, "KEY_DOWN");
                p.move(stdscr, Point(0, 1));
                break;
            case KEY_LEFT:
                mvwprintw(stdscr, 10, 40, "KEY_LEFT");
                p.move(stdscr, Point(-1, 0));
                break;
            case KEY_RIGHT:
                mvwprintw(stdscr, 10, 40, "KEY_RIGHT");
                p.move(stdscr, Point(1, 0));
                break;
        }
        for (std::vector<Enemy>::iterator it = enemies.begin();
             it != enemies.end(); ++it) {
            it->update();
            put_entity(stdscr, *it);
        }
        wrefresh(stdscr);
    }
    cleanup_and_exit();
}

int cleanup_and_exit(void) {
    endwin();
    std::exit(0);
}
