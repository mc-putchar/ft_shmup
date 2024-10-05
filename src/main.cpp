#include <curses.h>
#include <iostream>
#include "Entity.hpp"
#include "Player.hpp"

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;
    (void)initscr();
    keypad(stdscr, TRUE);
    (void)nonl();
    (void)cbreak();
    (void)noecho();
    Texture skin = Texture(10, 3, "~>L-\\___  ~XE[]==O}>~>F-/```  ");
    Player p = Player(Point(20, 20), 10, 10, skin);
    int c;
    bool run = true;
    while (run) {
        c = wgetch(stdscr);
        switch (c) {
            case KEY_UP:
                p.move(stdscr, Point(0, -1));
                break;
            case KEY_DOWN:
                p.move(stdscr, Point(0, 1));
                break;
            case KEY_LEFT:
                p.move(stdscr, Point(-1, 0));
                break;
            case KEY_RIGHT:
                p.move(stdscr, Point(1, 0));
                break;
            case 27:
                run = false;
                break;
        }
        // werase(stdscr);
        wrefresh(stdscr);
    }
    endwin();
    return (0);
}
