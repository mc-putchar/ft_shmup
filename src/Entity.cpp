////////////////////////////////////////////////////////////////////////////////
//       :::::::::: ::::    ::: ::::::::::: ::::::::::: ::::::::::: :::   ::: //
//      :+:        :+:+:   :+:     :+:         :+:         :+:     :+:   :+:  //
//     +:+        :+:+:+  +:+     +:+         +:+         +:+      +:+ +:+    //
//    +#++:++#   +#+ +:+ +#+     +#+         +#+         +#+       +#++:      //
//   +#+        +#+  +#+#+#     +#+         +#+         +#+        +#+        //
//  #+#        #+#   #+#+#     #+#         #+#         #+#        #+#         //
// ########## ###    ####     ###     ###########     ###        ###          //
////////////////////////////////////////////////////////////////////////////////

#include "Entity.hpp"

Entity::Entity(Point const& position, Point const& size, uint16_t hp,
               uint16_t sp, Texture const& texture)
    : position(position),
      width(size.x),
      height(size.y),
      health(hp),
      shield(sp),
      texture(texture) {}

Entity::Entity(Entity const& cpy)
    : position(cpy.position),
      width(cpy.width),
      height(cpy.height),
      health(cpy.health),
      shield(cpy.shield),
      texture(cpy.texture) {}

Entity& Entity::operator=(Entity const& rhs) {
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

void Entity::move(Point const& direction) {
    this->position += direction;
}

void Entity::take_damage(int amount) {
    if (amount > 0) {
        if (static_cast<uint16_t>(amount) <= this->health)
            this->health -= static_cast<uint16_t>(amount);
        else
            this->health = 0;
    }
}

void Entity::attack(Entity& target) {
    (void)target;
}

Point const& Entity::get_position() const {
    return this->position;
}

Point Entity::get_size() const {
    return Point(this->width, this->height);
}

uint16_t Entity::get_health() const {
    return this->health;
}

uint16_t Entity::get_shield() const {
    return this->shield;
}

void Entity::set_position(Point const& new_position) {
    this->position = new_position;
}

void Entity::set_health(uint16_t hp) {
    this->health = hp;
}

void Entity::set_shield(uint16_t sp) {
    this->shield = sp;
}

void Entity::set_skin(Texture const& texture) {
    this->texture = texture;
}

void Entity::set_weapon(Weapon* new_weapon) {
    // TODO: should we free previous weapon?
    this->weapon = new_weapon;
}
