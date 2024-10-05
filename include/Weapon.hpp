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
    Weapon();
    Weapon(Weapon const& cpy);
    Weapon& operator=(Weapon const& rhs);
    ~Weapon();
    void damage(Entity& target);
    bool shoot();

    std::vector<Projectile> projectiles;

  private:
    uint16_t cooldown;
    uint16_t last_shot;
};
