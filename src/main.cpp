#include <curses.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include "Entity.hpp"
#include "Player.hpp"
#include "ft_shmup.hpp"

pthread_mutex_t key_pressed_mtx = PTHREAD_MUTEX_INITIALIZER;

static void game_loop(Game& world, Player& p);

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;

    Game world;
    init_screen(world);
    mvwprintw(stdscr, 1, 1, "Hello, world!");
    refresh();
    wrefresh(world.main);

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
    while (true) {
        ch = wgetch(world.main);
        if (i >= 5) {
            wclear(world.main);
            wclear(world.hud);
            i = 0;
        }
        // if (ch != ERR) {
        switch (ch) {
            case 'w':
                mvwprintw(world.main, 10, 40, "KEY_UP");
                p.move(world.main, Point(0, -1));
                break;
            case 's':
                mvwprintw(world.main, 10, 40, "KEY_DOWN");
                p.move(world.main, Point(0, 1));
                break;
            case 'a':
                mvwprintw(world.main, 10, 40, "KEY_LEFT");
                p.move(world.main, Point(-1, 0));
                break;
            case 'd':
                mvwprintw(world.main, 10, 40, "KEY_RIGHT");
                p.move(world.main, Point(1, 0));
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
