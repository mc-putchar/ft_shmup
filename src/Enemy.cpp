////////////////////////////////////////////////////////////////////////////////
//            :::::::::: ::::    ::: ::::::::::   :::   :::  :::   :::        //
//           :+:        :+:+:   :+: :+:         :+:+: :+:+: :+:   :+:         //
//          +:+        :+:+:+  +:+ +:+        +:+ +:+:+ +:+ +:+ +:+           //
//         +#++:++#   +#+ +:+ +#+ +#++:++#   +#+  +:+  +#+  +#++:             //
//        +#+        +#+  +#+#+# +#+        +#+       +#+   +#+               //
//       #+#        #+#   #+#+# #+#        #+#       #+#   #+#                //
//      ########## ###    #### ########## ###       ###   ###                 //
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <iostream>
#include <vector>

#include "Enemy.hpp"

Enemy::Enemy(Point const& position, uint16_t hp, uint16_t sp,
             Texture const& texture)
    : Entity(position, hp, sp, texture), state(SPAWNING), ticks(0) {}

Enemy::Enemy(Enemy const& cpy) : Entity(cpy), state(cpy.state) {}

Enemy& Enemy::operator=(Enemy const& rhs) {
    if (this == &rhs)
        return *this;
    this->position = rhs.position;
    this->height = rhs.height;
    this->width = rhs.width;
    this->health = rhs.health;
    this->shield = rhs.shield;
    this->texture = rhs.texture;
    this->state = rhs.state;
    return *this;
}

Enemy::~Enemy() {
    if (this->weapon)
        delete this->weapon;
}

void Enemy::fire(std::vector<Projectile*>& bullets, int frame) {
    Point firepos;
    firepos.x = this->position.x - 3;
    firepos.y = this->position.y + this->texture.height / 2;
    Projectile* p = this->weapon->shoot(Point(-1, 0), firepos, frame);
    if (p) {
        bullets.push_back(p);
    }
}

void Enemy::update(std::vector<Projectile*>& bullets, int frame, int16_t x) {
    if (++this->ticks < 10)
        return;
    this->ticks = 0;
    this->Entity::repos();
    if (this->position.x + 5 < x && this->weapon) {
        this->fire(bullets, frame);
    }
}

void Enemy::die(void) {
    this->state = DEAD;
    return;
}

void Enemy::create_enemies(std::vector<Enemy>& enemies, int n) {
    // ,_,
    // |!|
    // ```
    Texture bullet_tex(1, 1, "-");
    Texture bomb_tex(1, 1, "*");
    Texture laser_icon(3, 3, ",_,|!|```");
    Texture pisciner_tex(7, 1, " (0){~ ");
    Texture peer_tex(5, 3, " -\\  <=B~  -/  ");
    Texture intern_tex(8, 3, " /--\\,<~G[II]XK~ \\--/`<~");
    int16_t x(150);
    Point dir(-1, 0);
    for (int i = 0; i < n; ++i) {
        Weapon* laser = new Weapon(laser_icon, 200, 1, bullet_tex, 1);
        Point p(x, ((i / 10) & 1) ? 10 : 20);
        Enemy enemy(p, 3, 0, pisciner_tex);
        enemy.set_direction(dir);
        // Pisciner& pisciner = static_cast<Pisciner&>(enemy);
        enemy.set_weapon(laser);
        enemies.push_back(enemy);
        x += 22;
    }
    x = 200;
    Weapon* laser;
    for (int i = 0; i < n / 2; ++i) {
        Point p(x, (i & 1) ? 5 : ((i & 3) ? 15 : 10));
        Enemy enemy(p, 5, 0, peer_tex);
        enemy.set_direction(dir);
        // Peer& peer = static_cast<Peer&>(enemy);
        laser = new Weapon(laser_icon, 100, 1, bullet_tex, 1);
        enemy.Entity::set_weapon(laser);
        enemies.push_back(enemy);
        x += 45;
    }
    x = 250;
    for (int i = 0; i < n / 4; ++i) {
        Point p(x, (i & 1) ? 21 : ((i & 3) ? 6 : 14));
        Enemy enemy(p, 7, 0, intern_tex);
        enemy.set_direction(dir);
        // Peer& peer = static_cast<Peer&>(enemy);
        laser = new Weapon(laser_icon, 80, 1, bomb_tex, 1);
        enemy.Entity::set_weapon(laser);
        enemies.push_back(enemy);
        x += 65;
    }
}
