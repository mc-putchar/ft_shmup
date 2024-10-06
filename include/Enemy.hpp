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
#include "Weapon.hpp"

#define ROCKET "🚀"
#define PISCINER "🤿"

enum e_state { SPAWNING, IDLE, ACTIVE, ATTACKING, DEFENDING, HURT, DEAD };

class Enemy : public Entity {
  public:
    Enemy(Point const& position, uint16_t hp, uint16_t sp,
          Texture const& texture);
    Enemy(Enemy const& cpy);
    Enemy& operator=(Enemy const& rhs);
    ~Enemy();

    virtual void attack(Entity& target);
    virtual void fire();
    virtual void update();
    virtual void die();
    static void create_enemies(std::vector<Enemy>& enemies, int n);

  protected:
    e_state state;

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
