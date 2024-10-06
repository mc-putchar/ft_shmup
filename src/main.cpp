
#include <curses.h>
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>  // for usleep

#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>  // for system()
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "Enemy.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "ft_shmup.hpp"

pthread_mutex_t key_pressed_mtx = PTHREAD_MUTEX_INITIALIZER;

static void game_loop(Game& world, Player& p);

void display_story(GameConfig& config, int rows, int cols) {
    (void)cols;
    std::string line;
    int i = 0;
    if ((int)config.intro.size() == 0) {
        mvprintw(0, 0, "...loading story...");
    }
    if ((int)config.intro.size() > rows) {
        attron(A_BLINK);
        mvprintw(i + 1, 15, "%s", "Press Any Keys To Continue");
        attroff(A_BLINK);
        refresh();
    } else {
        for (const std::string& line : config.intro) {
            mvprintw(i + 1, 15, "%s", line.c_str());
            i++;
            refresh();
            usleep(50000);
        }
    }
    getch();
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

    usleep(100000);  // give time to alacritty to set up the terminal

    GameConfig& config = GameConfig::getConf();
    GameParams params;
    params.fps = 60.0;
    params.startTime = std::chrono::high_resolution_clock::now();

    Game world;
    world.status = STORY;
    init_screen(world);
    mvwprintw(stdscr, 0, 1, "ft_shmup");
    refresh();
    wrefresh(world.main);
    if (world.status == STORY) {
        display_story(config, LINES, COLS);
        world.status = MENU;
    }
    nodelay(stdscr, TRUE);  // non-blocking getch

    Texture skin(10, 3, "~>L-\\___  ~XE[]==O}>~>F-/```  ");
    Player p(Point(20, 20), 10, 10, skin);
    p.move(world.main, Point(0, 0));  // Draw player at start position

    game_loop(world, p);

    return 0;
}

static void game_loop(Game& world, Player& p) {
    nodelay(world.main, TRUE);  // Make wgetch non-blocking
    nodelay(world.hud, TRUE);   // Make wgetch non-blocking
    nodelay(stdscr, TRUE);      // Make wgetch non-blocking
    int ch;
    int i = 0;
    std::vector<Enemy> enemies;
    Enemy::create_enemies(enemies, 5);
    std::vector<Projectile> bullets;
    while (true) {
        for (std::vector<Enemy>::iterator it = enemies.begin();
             it != enemies.end(); ++it) {
            it->update(bullets);
            put_entity(world.main, *it);
        }
        for (std::vector<Projectile>::iterator it = bullets.begin();
             it != bullets.end(); ++it) {
            it->update();
            put_projectile(world.main, *it);
        }
        ch = wgetch(world.main);
        if (i >= 5) {
            wclear(world.main);
            wclear(world.hud);
            i = 0;
        }
        // if (ch != ERR) {
        switch (ch) {
            case 'w':
                // mvwprintw(world.main, 10, 40, "KEY_UP");
                p.move(world.main, Point(0, -1));
                break;
            case 's':
                // mvwprintw(world.main, 10, 40, "KEY_DOWN");
                p.move(world.main, Point(0, 1));
                break;
            case 'a':
                // mvwprintw(world.main, 10, 40, "KEY_LEFT");
                p.move(world.main, Point(-1, 0));
                break;
            case 'd':
                // mvwprintw(world.main, 10, 40, "KEY_RIGHT");
                p.move(world.main, Point(1, 0));
                break;
            case ' ':
                // mvwprintw(world.main, 10, 40, "KEY_SPACE");
                p.fire();
                break;
            case 10:  // Enter key
                break;
            case 27:  // Escape key
                cleanup_and_exit();
                break;
            default:
                p.move(world.main, Point(0, 0));
                wborder(world.main, 0, 0, 0, 0, 0, 0, 0, 0);
                wborder(world.hud, 0, 0, 0, 0, 0, 0, 0, 0);
                break;
        }
        // }
        refresh();
        wrefresh(world.main);
        wrefresh(world.hud);
        usleep(10000);  // Sleep for 20ms
        i++;
    }
}

int cleanup_and_exit(void) {
    endwin();
    printf("Exiting...\n");
    exit(0);
}
