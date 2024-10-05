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
#include <vector>
#include "Entity.hpp"

class Projectile {
  public:
    Projectile();
    ~Projectile();
    void update();

  private:
    Texture texture;
    Point position;
    Point direction;
    uint16_t speed;
};

class Weapon {
  public:
    Weapon(uint16_t cooldown, uint16_t last_shot)
        : cooldown(cooldown), last_shot(last_shot) {};
    Weapon(Weapon const& cpy)
        : cooldown(cpy.cooldown), last_shot(cpy.last_shot) {};
    Weapon& operator=(Weapon const& rhs);
    ~Weapon();
    void damage(Entity& target);
    bool shoot();

    std::vector<Projectile> projectiles;

  private:
    uint16_t cooldown;
    uint16_t last_shot;
};
