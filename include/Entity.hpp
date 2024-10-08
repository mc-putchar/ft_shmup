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

#include <curses.h>
#include <cstdint>
#include <string>
#include <vector>
#include "Game.hpp"

class Weapon;

struct Point {
    int16_t x;
    int16_t y;
    Point() : x(0), y(0) {};
    Point(int16_t x, int16_t y) : x(x), y(y) {};
    Point(Point const& cpy) : x(cpy.x), y(cpy.y) {};
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
    Point& operator-=(Point const& p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    };
    bool operator==(Point const& p) const {
        return this->x == p.x && this->y == p.y;
    };
    bool operator!=(Point const& p) const {
        return this->x != p.x || this->y != p.y;
    };
    bool operator>=(Point const& p) const {
        return this->x >= p.x && this->y >= p.y;
    };
    bool operator<=(Point const& p) const {
        return this->x <= p.x && this->y <= p.y;
    };
    Point operator+(Point const& p) const {
        return Point(this->x + p.x, this->y + p.y);
    };
    Point operator-(Point const& p) const {
        return Point(this->x - p.x, this->y - p.y);
    };
};

struct Texture {
    Texture(uint16_t w, uint16_t h, std::string const& data)
        : width(w), height(h), data(data) {};
    Texture(Texture const& cpy)
        : width(cpy.width), height(cpy.height), data(cpy.data) {};
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
    Entity(Point const& position, uint16_t hp, uint16_t sp,
           Texture const& texture);
    Entity(Entity const& cpy);
    Entity& operator=(Entity const& rhs);
    virtual ~Entity() = default;

    virtual void repos(void);
    virtual void take_damage(int amount);
    virtual void attack(Entity& target);

    Point const& get_position() const;
    Point const& get_direction() const;
    Point get_size() const;
    uint16_t get_health() const;
    uint16_t get_shield() const;
    Texture get_texture() const;

    void set_position(Point const& new_position);
    void set_direction(Point const& new_direction);
    void set_health(uint16_t hp);
    void set_shield(uint16_t sp);
    void set_texture(Texture const& skin);
    void set_weapon(Weapon* new_weapon);

    std::vector<Region> current_regions;

  protected:
    Point position;
    Point direction;
    uint16_t width;
    uint16_t height;
    uint16_t health;
    uint16_t shield;
    Texture texture;
    Weapon* weapon;
};
