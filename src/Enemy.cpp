////////////////////////////////////////////////////////////////////////////////
//            :::::::::: ::::    ::: ::::::::::   :::   :::  :::   :::        //
//           :+:        :+:+:   :+: :+:         :+:+: :+:+: :+:   :+:         //
//          +:+        :+:+:+  +:+ +:+        +:+ +:+:+ +:+ +:+ +:+           //
//         +#++:++#   +#+ +:+ +#+ +#++:++#   +#+  +:+  +#+  +#++:             //
//        +#+        +#+  +#+#+# +#+        +#+       +#+   +#+               //
//       #+#        #+#   #+#+# #+#        #+#       #+#   #+#                //
//      ########## ###    #### ########## ###       ###   ###                 //
////////////////////////////////////////////////////////////////////////////////

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

Enemy::~Enemy() {}

void Enemy::fire() {
    if (this->weapon) {
        (void)this->weapon->shoot(this->direction,
                                  this->position - Point(-1, 0), 0);
    }
}

void Enemy::update(std::vector<Projectile*>& bullets, int frame) {
    if (++this->ticks < 10)
        return;
    this->ticks = 0;
    this->Entity::repos();
    if (this->weapon) {
        Projectile* p =
            this->weapon->shoot(Point(-1, 0), this->position, frame);
        if (p) {
            bullets.push_back(p);
        }
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
    Texture laser_icon(3, 3, ",_,|!|```");
    Weapon laser(laser_icon, 1000, 1, bullet_tex, 3);
    Texture pisciner_tex(7, 1, " (0){~ ");
    Texture peer_tex(5, 3, " -\\  <=B~  -/  ");
    int16_t x(180);
    Point dir(-1, 0);
    for (int i = 0; i < n; ++i) {
        Point p(x, 20);
        Enemy enemy(p, 3, 0, pisciner_tex);
        enemy.set_direction(dir);
        Pisciner& pisciner = static_cast<Pisciner&>(enemy);
        pisciner.set_weapon(&laser);
        enemies.push_back(enemy);
        x += 15;
    }
    x = 240;
    for (int i = 0; i < n / 2; ++i) {
        Point p(x, 10);
        Enemy enemy(p, 5, 0, peer_tex);
        enemy.set_direction(dir);
        Peer& peer = static_cast<Peer&>(enemy);
        peer.set_weapon(&laser);
        enemies.push_back(enemy);
        x += 25;
    }
}
