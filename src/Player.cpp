////////////////////////////////////////////////////////////////////////////////
//          :::::::::  :::            :::   :::   ::: :::::::::: :::::::::    //
//         :+:    :+: :+:          :+: :+: :+:   :+: :+:        :+:    :+:    //
//        +:+    +:+ +:+         +:+   +:+ +:+ +:+  +:+        +:+    +:+     //
//       +#++:++#+  +#+        +#++:++#++: +#++:   +#++:++#   +#++:++#:       //
//      +#+        +#+        +#+     +#+  +#+    +#+        +#+    +#+       //
//     #+#        #+#        #+#     #+#  #+#    #+#        #+#    #+#        //
//    ###        ########## ###     ###  ###    ########## ###    ###         //
////////////////////////////////////////////////////////////////////////////////

#include "Player.hpp"
#include <curses.h>
#include <iostream>

Player::Player(Point start_pos, uint16_t hp, uint16_t sp, Texture texture)
    : Entity(start_pos, hp, sp, texture) {
    this->set_position(start_pos);
    this->set_health(hp);
    this->set_shield(shield);
    this->set_texture(texture);
    this->width = texture.width;
    this->height = texture.height;
    this->weapon = nullptr;
}

Player::Player(Player const& cpy) : Entity(cpy) {
    if (this == &cpy)
        return;
    this->set_position(cpy.get_position());
    this->set_health(cpy.get_health());
    this->set_shield(cpy.get_shield());
    this->set_texture(cpy.get_texture());
    this->width = cpy.width;
    this->height = cpy.height;
    // this->weapon = cpy.weapon ? new Weapon(*cpy.weapon) : nullptr;
}

Player& Player::operator=(Player const& rhs) {
    if (this == &rhs)
        return *this;
    this->set_position(rhs.get_position());
    this->set_health(rhs.get_health());
    this->set_shield(rhs.get_shield());
    this->set_texture(rhs.get_texture());
    this->width = rhs.width;
    this->height = rhs.height;
    // this->weapon = rhs.weapon ? new Weapon(*rhs.weapon) : nullptr;
    return *this;
}

Player::~Player() {
    if (this->weapon)
        delete this->weapon;
}

void Player::fire(std::vector<Projectile*>& bullets, int frame) {
    Point firepoint;

    firepoint.x = this->position.y + (this->texture.height / 2);
    firepoint.y = this->position.x + this->texture.width;
    if (this->weapon) {
        Projectile* bullet =
            this->weapon->shoot(Point(0, 1), firepoint, frame);
        if (bullet)
            bullets.push_back(bullet);
    }
}

void Player::attack(Entity& target) {
    (void)target;
};

void Player::take_damage(int amount) {}

void Player::move(Game const& world, Point const& direction) {
    uint16_t mw, mh;
    getmaxyx(world.main, mh, mw);
    if (this->position.x + direction.x + this->texture.width >= mw ||
        this->position.x + direction.x <= 0 ||
        this->position.y + direction.y + this->texture.height >= mh ||
        this->position.y + direction.y <= 0) {
        this->_display(world.main);
        return;
    }
    this->position += direction;
    // if (this->current_regions.size() > 0) {
    //     this->current_regions.clear();
    //     for (Region& r : this->current_regions) {
    //         if (!r.is_point_inside(this->position)) {
    //             for (Region adj : r.get_adjacent_regions(world)) {
    //                 if (adj.is_point_inside(this->position)) {
    //                     this->current_regions.push_back(adj);
    //                 } else if (adj.is_point_inside(
    //                                this->position +
    //                                Point(0, this->texture.height))) {
    //                     this->current_regions.push_back(adj);
    //                 } else if (adj.is_point_inside(
    //                                this->position +
    //                                Point(this->texture.width, 0))) {
    //                     this->current_regions.push_back(adj);
    //                 } else if (adj.is_point_inside(
    //                                this->position +
    //                                Point(this->texture.width,
    //                                      this->texture.height))) {
    //                     this->current_regions.push_back(adj);
    //                 }
    //             }
    //         }
    //     }
    // }
    this->_display(world.main);
}

void Player::_display(WINDOW* win) const {
    uint16_t cursor = 0;
    for (uint16_t row = 0; row < this->texture.height; ++row) {
        mvwprintw(
            win, this->position.y + row, this->position.x, "%s",
            this->texture.data.substr(cursor, this->texture.width).c_str());
        cursor += this->texture.width;
    }
}