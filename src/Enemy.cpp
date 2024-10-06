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

Enemy::Enemy(Point const& position, Point const& size, uint16_t hp, uint16_t sp,
             Texture const& texture)
    : Entity(position, size, hp, sp, texture) {}

Enemy::Enemy(Enemy const& cpy) : Entity(cpy) {}

Enemy& Enemy::operator=(Enemy const& rhs) {
    if (this == &rhs)
        return *this;
    this->position = rhs.position;
    this->height = rhs.height;
    this->width = rhs.width;
    this->health = rhs.health;
    this->shield = rhs.shield;
    this->texture = rhs.texture;
    return *this;
}

Enemy::~Enemy() {}

void Enemy::attack(Entity& target) {
    if (this->weapon) {
        target.take_damage(1);
        std::cerr << "Watch out! They've got guns!" << std::endl;
    }
}

void Enemy::fire() {
    if (this->weapon) {
        (void)this->weapon->shoot(this->direction, this->position, 0);
    }
}

void Enemy::update(void) {
    Point next_dir(3, 2);
    this->set_direction(next_dir);
    this->Entity::move();
    for (std::vector<Projectile>::iterator it =
             this->weapon->projectiles.begin();
         it != this->weapon->projectiles.end(); ++it) {
        it->update();
    }
}

void Enemy::die(void) {
    return;
}
