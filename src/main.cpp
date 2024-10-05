#include <curses.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "Entity.hpp"
#include "Player.hpp"
#include "ft_shmup.hpp"

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;

    (void)initscr();
    keypad(stdscr, TRUE);
    (void)nonl();
    (void)cbreak();
    (void)noecho();
    (void)curs_set(0);  // Disable cursor

    Texture skin(10, 3, "~>L-\\___  ~XE[]==O}>~>F-/```  ");
    Player p(Point(20, 20), 10, 10, skin);
    p.move(stdscr, Point(0, 0));  // Draw player at start position

    // Idea for control:
    // - Detect key down and key up events
    //    - If key down, toggle the flag for that key
    //    - If key up, toggle the flag back to false fot that key
    // - In the main loop, check the flags and move the player accordingly

    // Primitive control loop
    int c;
    while (true) {
        c = wgetch(stdscr);
        wclear(stdscr);
        switch (c) {
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
            case 27:
                cleanup_and_exit();
                break;
        }
        wrefresh(stdscr);
    }
    cleanup_and_exit();
}

int cleanup_and_exit(void) {
    endwin();
    exit(0);
}
