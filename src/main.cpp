
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>  // for usleep
#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>  // for system()
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>

#include "Enemy.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"
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

typedef struct {
    double fps;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    std::chrono::duration<double> elapsedTime;
    std::chrono::duration<double> remainingTime;
} GameParams;

char get_wall_piece(int n) {
    static char const* wall_segs = ".,/_\\`-;:";
    return wall_segs[n % 10];
}

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;

    std::signal(SIGINT, ft_shmup::stop_game);
    std::signal(SIGTERM, ft_shmup::stop_game);
    usleep(100000);  // give time to alacritty to set up the terminal
    GameConfig& config = GameConfig::getConf();
    GameParams params;

    // init_game();
    Game game;
    init_screen(game);

    params.fps = 60.0;
    params.startTime = std::chrono::high_resolution_clock::now();
    // auto lastTime = startTime;
    int c = 0;
    WINDOW* my_pad = newpad(LINES, 300);
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < COLS * 2; ++x) {
            uint8_t ch = get_wall_piece(rand() % 128);
            mvwaddch(my_pad, y, x, ch);  //
        }
    }
    for (int y = LINES - 1; y > LINES - 5; --y) {
        for (int x = 0; x < COLS * 2; ++x) {
            uint8_t ch = get_wall_piece(rand() % 128);
            mvwaddch(my_pad, y, x, ch);  //
        }
    }

    Texture skin(10, 3, "~>L-\\___  ~XE[]==O}>~>F-/```  ");
    Player p(Point(20, 20), 10, 10, skin);
    // p.move(stdscr, Point(0, 0));  // Draw player at start position

    std::vector<Enemy> enemies;
    Enemy::create_enemies(enemies, 5);

    int offset = 0;
    while ((c = getch()) != 27) {
        if (!config.storyPlayed) {
            nodelay(stdscr, FALSE);
            display_story(config, LINES, COLS);
            mvwprintw(stdscr, 0, LINES - 10, "%s", "ft_shmup");
            if ((c) != 27 && (c) != ERR) {
                config.storyPlayed = true;
                nodelay(stdscr, TRUE);
                continue;
            } else if ((c) != ERR) {
                break;
            }

        } else {
            switch (c) {
                case KEY_ESC:
                    cleanup_and_exit();
                    break;
                case KEY_UP:
                    // mvwprintw(stdscr, 10, 40, "KEY_UP");
                    p.move(my_pad, Point(0, -1));
                    break;
                case KEY_DOWN:
                    // mvwprintw(stdscr, 10, 40, "KEY_DOWN");
                    p.move(my_pad, Point(0, 1));
                    break;
                case KEY_LEFT:
                    // mvwprintw(stdscr, 10, 40, "KEY_LEFT");
                    p.move(my_pad, Point(-1, 0));
                    break;
                case KEY_RIGHT:
                    // mvwprintw(stdscr, 10, 40, "KEY_RIGHT");
                    p.move(my_pad, Point(1, 0));
                    break;
                default:
                    p.move(my_pad, Point(0, 0));
                    break;
            }
            // werase(stdscr);
            // pnoutrefresh(my_pad, 0, offset, 0, 0, LINES - 1, COLS - 1);

            usleep(50000);
            // p.move(my_pad, Point(offset, 0));
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
            params.remainingTime =
                std::chrono::duration<double>(1.0 / 60.0) - params.elapsedTime;
            if (params.remainingTime > std::chrono::duration<double>(0.0)) {
                std::this_thread::sleep_for(params.remainingTime);
            } else {
                mvwprintw(stdscr, 0, COLS - 10, "fps ...");
            }

            for (std::vector<Enemy>::iterator it = enemies.begin();
                 it != enemies.end(); ++it) {
                it->update();
                put_entity(my_pad, *it);
            }
        }
    }
    cleanup_and_exit();
    return (0);
}

int cleanup_and_exit(void) {
    endwin();
    std::exit(0);
}
