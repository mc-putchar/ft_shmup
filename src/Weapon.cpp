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
                       uint16_t speed)
    : texture(tex), position(pos), direction(dir), speed(speed) {}

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

std::string const& Projectile::get_texture() const {
	return this->texture.data;
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
               Texture const& projectile)
    : projectiles(),
      last_shot(0),
      cooldown(cooldown),
      damage(damage),
      icon(icon),
      projectile_texture(projectile) {}

Weapon::Weapon(Weapon const& cpy)
    : projectiles(),
      last_shot(cpy.last_shot),
      cooldown(cpy.cooldown),
      damage(cpy.damage),
      icon(cpy.icon),
      projectile_texture(cpy.projectile_texture) {}

Weapon& Weapon::operator=(Weapon const& rhs) {
    if (this == &rhs)
        return *this;
    this->last_shot = rhs.last_shot;
    this->cooldown = rhs.cooldown;
    this->damage = rhs.damage;
    this->icon = rhs.icon;
    this->projectile_texture = rhs.projectile_texture;
    this->projectiles.clear();
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
    return this->damage;
}

Texture const& Weapon::get_icon() const {
    return this->icon;
}

void Weapon::do_damage(Entity& target) {
    target.take_damage(this->damage);
}

bool Weapon::shoot(Point const& dir, Point const& start, uint32_t time) {
    if (static_cast<uint32_t>(this->cooldown) > time - this->last_shot)
        return false;
    this->projectiles.push_back(Projectile(this->projectile_texture, start, dir,
                                           this->projectile_speed));
    return true;
}
