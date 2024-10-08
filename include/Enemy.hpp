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

    virtual void fire(std::vector<Projectile*>& bullets, int frame);
    virtual void update(std::vector<Projectile*>& bullets, int frame,
                        int16_t x);
    virtual void die();
    static void create_enemies(std::vector<Enemy>& enemies, int n);
    bool operator==(const Enemy& other) const {
        // Compare relevant members for equality
        return this == &other;  // Example comparison
    }

  protected:
    e_state state;
    int ticks;
    int shoot;

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
