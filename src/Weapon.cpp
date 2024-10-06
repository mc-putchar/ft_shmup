////////////////////////////////////////////////////////////////////////////////
//     :::       ::: ::::::::::     :::     :::::::::   ::::::::  ::::    ::: //
//    :+:       :+: :+:          :+: :+:   :+:    :+: :+:    :+: :+:+:   :+:  //
//   +:+       +:+ +:+         +:+   +:+  +:+    +:+ +:+    +:+ :+:+:+  +:+   //
//  +#+  +:+  +#+ +#++:++#   +#++:++#++: +#++:++#+  +#+    +:+ +#+ +:+ +#+    //
// +#+ +#+#+ +#+ +#+        +#+     +#+ +#+        +#+    +#+ +#+  +#+#+#     //
// #+#+# #+#+#  #+#        #+#     #+# #+#        #+#    #+# #+#   #+#+#      //
// ###   ###   ########## ###     ### ###         ########  ###    ####       //
////////////////////////////////////////////////////////////////////////////////

#include "Weapon.hpp"

////////////////////////////////////////////////////////////////////////////////
// Projectile

Projectile::Projectile(Texture const& tex, Point const& pos, Point const& dir,
                       int16_t damage, uint16_t speed)
    : texture(tex),
      position(pos),
      direction(dir),
      damage(damage),
      speed(speed),
      active(true) {}

Projectile::~Projectile() {}

Point const& Projectile::get_position() const {
    return this->position;
}

Point const& Projectile::get_direction() const {
    return this->direction;
}

Point Projectile::get_size() const {
    return Point(this->texture.width, this->texture.height);
}

uint16_t Projectile::get_speed() const {
    return this->speed;
}

Texture Projectile::get_texture() const {
    return this->texture;
}

void Projectile::set_direction(Point const& new_dir) {
    this->direction = new_dir;
}

void Projectile::update() {
    uint16_t x = this->direction.x * this->speed;
    uint16_t y = this->direction.y * this->speed;
    this->position.x += x;
    this->position.y += y;
}

////////////////////////////////////////////////////////////////////////////////
// Weapon

Weapon::Weapon(Texture const& icon, uint16_t cooldown, int16_t damage,
               Texture const& projectile, uint16_t speed)
    : last_shot(0),
      cooldown(cooldown),
      icon(icon),
      projectile_damage(damage),
      projectile_texture(projectile),
      projectile_speed(speed) {}

Weapon::Weapon(Weapon const& cpy)
    : last_shot(cpy.last_shot),
      cooldown(cpy.cooldown),
      icon(cpy.icon),
      projectile_damage(cpy.projectile_damage),
      projectile_texture(cpy.projectile_texture),
      projectile_speed(cpy.projectile_speed) {}

Weapon& Weapon::operator=(Weapon const& rhs) {
    if (this == &rhs)
        return *this;
    this->last_shot = rhs.last_shot;
    this->cooldown = rhs.cooldown;
    this->icon = rhs.icon;
    this->projectile_damage = rhs.projectile_damage;
    this->projectile_texture = rhs.projectile_texture;
    this->projectile_speed = rhs.projectile_speed;
    return *this;
}

Weapon::~Weapon() {}

uint16_t Weapon::get_cooldown() const {
    return this->cooldown;
}

uint16_t Weapon::get_last_shot() const {
    return this->last_shot;
}

int16_t Weapon::get_damage() const {
    return this->projectile_damage;
}

Texture const& Weapon::get_icon() const {
    return this->icon;
}

void Weapon::do_damage(Entity& target) {
    target.take_damage(this->projectile_damage);
}

Projectile* Weapon::shoot(Point const& dir, Point const& start, uint32_t time) {
    // if (static_cast<uint32_t>(this->cooldown) > time - this->last_shot)
    //     return nullptr;
    this->last_shot = time;
    return new Projectile(this->projectile_texture, start, dir,
                this->projectile_damage,
                this->projectile_speed);
}
