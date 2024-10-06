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

#include "Entity.hpp"
#include "Weapon.hpp"
#include <cstdint>
#include <curses.h>

class Player : public Entity {
  public:
    Player(Point start_pos, uint16_t hp, uint16_t sp, Texture texture);
    Player(Player const& cpy);
    Player& operator=(Player const& rhs);
    ~Player();
    void move(WINDOW *win, Point const& direction); // TODO: fix problem with diff definition of move function
    void attack(Entity& target);
    void fire(void);
    void take_damage(int amount);

  private:
    void _display(WINDOW *win) const;
};
