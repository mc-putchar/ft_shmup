#include <curses.h>

#include "Entity.hpp"
#include "Game.hpp"
#include "Weapon.hpp"

void init_screen(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    nonl();
    cbreak();
    noecho();
    setlocale(LC_ALL, "");
    start_color();                             // allow colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);     // color pair 1
    init_pair(2, COLOR_RED, COLOR_BLACK);      // color pair 2
    init_pair(3, COLOR_GREEN, COLOR_BLACK);    // color pair 3
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);   // color pair 4
    init_pair(5, COLOR_BLUE, COLOR_BLACK);     // color pair 5
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);  // color pair 6
    init_pair(7, COLOR_WHITE, COLOR_BLACK);    // color pair 7
}

void init_windows(Game& game) {
    int sx;
    int sy;

    getmaxyx(stdscr, sy, sx);
    game.main = newwin((sy / 4) * 3, sx, 0,
                       0);  // Create a new window for the game world
    game.hud =
        newwin(sy / 4, sx, (sy / 4) * 3, 0);  // Create a new window for the HUD
    refresh();
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

void put_projectile(WINDOW* win, Projectile* p) {
    int sx;
    int sy;

    getmaxyx(win, sy, sx);
    Point pos(p->get_position());
    if (pos.y < 0 || pos.y >= sx || pos.x < 0 || pos.x >= sy) {
        p->active = false;
        return;
    }
    Point size(p->get_size());
    char const* tex(p->get_texture().data.c_str());
    for (uint16_t i = 0; i < size.y; ++i) {
        mvwprintw(win, pos.x, pos.y, "%.*s", size.x, &(tex[i * size.x]));
    }
}
