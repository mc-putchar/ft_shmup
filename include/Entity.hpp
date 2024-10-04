////////////////////////////////////////////////////////////////////////////////
//       :::::::::: ::::    ::: ::::::::::: ::::::::::: ::::::::::: :::   ::: //
//      :+:        :+:+:   :+:     :+:         :+:         :+:     :+:   :+:  //
//     +:+        :+:+:+  +:+     +:+         +:+         +:+      +:+ +:+    //
//    +#++:++#   +#+ +:+ +#+     +#+         +#+         +#+       +#++:      //
//   +#+        +#+  +#+#+#     +#+         +#+         +#+        +#+        //
//  #+#        #+#   #+#+#     #+#         #+#         #+#        #+#         //
// ########## ###    ####     ###     ###########     ###        ###          //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <string>

class Weapon;

struct Point {
  uint16_t x;
  uint16_t y;
};

struct Texture {
  uint16_t width;
  uint16_t height;
  std::string data;
};

class Entity {
public:
  virtual ~Entity();
  virtual void move(Point const &direction);
  virtual void attack(Entity &target);
  virtual void take_damage(int amount);
  virtual void change_weapon(Weapon const *new_weapon);

private:
  Point position;
  uint16_t width;
  uint16_t height;
  uint16_t health;
  uint16_t shield;
  Texture skin;
  Weapon *weapon;
};
