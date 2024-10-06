#pragma once

#include "Game.hpp"
#include "Weapon.hpp"

void init_screen(Game& game);
void put_entity(WINDOW* win, Entity const& entity);
void put_projectile(WINDOW* win, Projectile const& p);
int cleanup_and_exit(void);