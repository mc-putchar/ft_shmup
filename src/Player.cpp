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

void Player::attack(Entity& target) {
    (void)target;
}

void Player::take_damage(int amount) {}

void Player::move(WINDOW* win, Point const& direction) {
    uint16_t mw, mh;
    getmaxyx(win, mh, mw);
    mvwprintw(win, 5, 5, "max: %d %d, pos: %d %d\n", mw, mh, this->position.x,
              this->position.y);
    if (this->position.x + direction.x + this->texture.width >= mw ||
        this->position.x + direction.x <= 0 ||
        this->position.y + direction.y + this->texture.height >= mh ||
        this->position.y + direction.y <= 0) {
        this->_display(win);
        return;
    }
    this->position += direction;
    this->_display(win);
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