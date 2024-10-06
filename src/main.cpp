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
