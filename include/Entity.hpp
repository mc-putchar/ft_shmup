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
    Point(uint16_t x, uint16_t y) : x(x), y(y){};
    Point(Point const& cpy) : x(cpy.x), y(cpy.y){};
    Point& operator=(Point const& rhs) {
        if (this == &rhs)
            return *this;
        this->x = rhs.x;
        this->y = rhs.y;
        return *this;
    };
    Point& operator+=(Point const& p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    };
};

struct Texture {
    Texture(uint16_t w, uint16_t h, std::string const& data)
        : width(w), height(h), data(data) {}
    Texture& operator=(Texture const& rhs) {
        if (this == &rhs)
            return *this;
        this->width = rhs.width;
        this->height = rhs.height;
        this->data = rhs.data;
        return *this;
    }
    uint16_t width;
    uint16_t height;
    std::string data;
};

class Entity {
  public:
    virtual ~Entity() = default;
    virtual void move(Point const& direction);
    virtual void take_damage(int amount);
    virtual void attack(Entity& target);

    Point const& get_position() const;
    Point get_size() const;
    uint16_t get_health() const;
    uint16_t get_shield() const;

    void set_position(Point const& new_position);
    void set_health(uint16_t hp);
    void set_shield(uint16_t sp);
    void set_skin(Texture const& skin);
    void set_weapon(Weapon* new_weapon);

  protected:
    Point position;
    uint16_t width;
    uint16_t height;
    uint16_t health;
    uint16_t shield;
    Texture texture;
    Weapon* weapon;
};
