////////////////////////////////////////////////////////////////////////////////
//     :::       ::: ::::::::::     :::     :::::::::   ::::::::  ::::    ::: //
//    :+:       :+: :+:          :+: :+:   :+:    :+: :+:    :+: :+:+:   :+:  //
//   +:+       +:+ +:+         +:+   +:+  +:+    +:+ +:+    +:+ :+:+:+  +:+   //
//  +#+  +:+  +#+ +#++:++#   +#++:++#++: +#++:++#+  +#+    +:+ +#+ +:+ +#+    //
// +#+ +#+#+ +#+ +#+        +#+     +#+ +#+        +#+    +#+ +#+  +#+#+#     //
// #+#+# #+#+#  #+#        #+#     #+# #+#        #+#    #+# #+#   #+#+#      //
// ###   ###   ########## ###     ### ###         ########  ###    ####       //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Entity.hpp"

class Projectile {
  public:
    Projectile(Texture const& tex, Point const& pos, Point const& dir,
               int16_t damage, uint16_t speed);
    ~Projectile();

    Point const& get_position() const;
    Point const& get_direction() const;
    Point get_size() const;
    uint16_t get_speed() const;
    Texture get_texture() const;

    void set_direction(Point const& new_dir);

    void update();
    bool operator==(const Projectile& other) const {
        // Compare relevant members for equality
        return this == &other;  // Example comparison
    }

  private:
    Texture texture;
    Point position;
    Point direction;
    int16_t damage;
    uint16_t speed;
};

class Weapon {
  public:
    Weapon(Texture const& icon, uint16_t cooldown, int16_t damage,
           Texture const& projectile, uint16_t speed);
    Weapon(Weapon const& cpy);
    Weapon& operator=(Weapon const& rhs);
    ~Weapon();

    uint16_t get_cooldown() const;
    uint16_t get_last_shot() const;
    int16_t get_damage() const;
    Texture const& get_icon() const;

    void do_damage(Entity& target);
    Projectile* shoot(Point const& dir, Point const& start, uint32_t time);

  private:
    uint32_t last_shot;
    uint16_t cooldown;
    Texture icon;
    int16_t projectile_damage;
    Texture projectile_texture;
    uint16_t projectile_speed;
};
