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
    curs_set(0);
    nonl();
    cbreak();
    noecho();
    getmaxyx(stdscr, sy, sx);
    game.main = newwin((sy / 4) * 3, sx, 0, 0);  // Create a new window for the game world
    game.hud = newwin(sy / 4, sx, (sy / 4) * 3, 0);  // Create a new window for the HUD
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
        mvwprintw(win, p.x, p.y + i, "%.*s", s.x, &(tex[i * s.x]));
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
