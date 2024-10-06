#include <curses.h>

#include "Entity.hpp"
#include "Game.hpp"
#include "Weapon.hpp"

void init_screen(Game& game) {
    int sx;
    int sy;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  // non-blocking getch
    curs_set(0);
    nonl();
    cbreak();
    noecho();
    start_color();                          // allow colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // color pair 1
    getmaxyx(stdscr, sx, sy);
    game.main = subwin(stdscr, sy / 2, sx, 0, 0);
    game.hud = subwin(stdscr, sy / 2, sx, sy / 2, sx);
    refresh();
    border(0, 0, 0, 0, 0, 0, 0, 0);
    wborder(game.main, 0, 0, 0, 0, 0, 0, 0, 0);
    wborder(game.hud, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(game.main);
    wrefresh(game.hud);
}

void put_entity(WINDOW* win, Entity const& entity) {
    Point p(entity.get_position());
    Point s(entity.get_size());
    char const* tex(entity.get_texture().data.c_str());
    for (uint16_t i = 0; i < s.y; ++i) {
        mvwprintw(win, p.y + i, p.x, "%.*s", s.x, &(tex[i * s.x]));
    }
}

void put_projectile(WINDOW* win, Projectile const& p) {
    Point pos(p.get_position());
    Point size(p.get_size());
    char const* tex(p.get_texture().data.c_str());
    for (uint16_t i = 0; i < size.y; ++i) {
        mvwprintw(win, pos.x, pos.y, "%.*s", size.x, &(tex[i * size.x]));
    }
}
