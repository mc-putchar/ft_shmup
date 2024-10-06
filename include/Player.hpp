////////////////////////////////////////////////////////////////////////////////
//          :::::::::  :::            :::   :::   ::: :::::::::: :::::::::    //
//         :+:    :+: :+:          :+: :+: :+:   :+: :+:        :+:    :+:    //
//        +:+    +:+ +:+         +:+   +:+ +:+ +:+  +:+        +:+    +:+     //
//       +#++:++#+  +#+        +#++:++#++: +#++:   +#++:++#   +#++:++#:       //
//      +#+        +#+        +#+     +#+  +#+    +#+        +#+    +#+       //
//     #+#        #+#        #+#     #+#  #+#    #+#        #+#    #+#        //
//    ###        ########## ###     ###  ###    ########## ###    ###         //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <curses.h>
#include <cstdint>
#include "Entity.hpp"
#include "Game.hpp"
#include "Weapon.hpp"

class Player : public Entity {
  public:
    Player(Point start_pos, uint16_t hp, uint16_t sp, Texture texture);
    Player(Player const& cpy);
    Player& operator=(Player const& rhs);
    ~Player();
    void move(
        Game const& world,
        Point const&
            direction);  // TODO: fix problem with diff definition of move function
    void attack(Entity& target);
    void fire(void);
    void take_damage(int amount);

  private:
    void _display(WINDOW* win) const;
};
