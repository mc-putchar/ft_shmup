////////////////////////////////////////////////////////////////////////////////
//            :::::::::: ::::    ::: ::::::::::   :::   :::  :::   :::        //
//           :+:        :+:+:   :+: :+:         :+:+: :+:+: :+:   :+:         //
//          +:+        :+:+:+  +:+ +:+        +:+ +:+:+ +:+ +:+ +:+           //
//         +#++:++#   +#+ +:+ +#+ +#++:++#   +#+  +:+  +#+  +#++:             //
//        +#+        +#+  +#+#+# +#+        +#+       +#+   +#+               //
//       #+#        #+#   #+#+# #+#        #+#       #+#   #+#                //
//      ########## ###    #### ########## ###       ###   ###                 //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Entity.hpp"

class Enemy : public Entity {
  public:
    Enemy(Point const& position, Point const& size, uint16_t hp, uint16_t sp,
          Texture const& texture);
    Enemy(Enemy const& cpy);
    Enemy& operator=(Enemy const& rhs);
    ~Enemy();

    virtual void attack(Entity& target) = 0;
    virtual void update() = 0;

  private:
};

class Pisciner : public Enemy {
  public:
  private:
};

class Peer : public Enemy {
  public:
  private:
};

class Intern : public Enemy {
  public:
  private:
};

class Cheater : public Enemy {
  public:
  private:
};

class Boss : public Enemy {
  public:
  private:
};
