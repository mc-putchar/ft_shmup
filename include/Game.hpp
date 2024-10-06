////////////////////////////////////////////////////////////////////////////////
//                 ::::::::      :::       :::   :::   ::::::::::             //
//               :+:    :+:   :+: :+:    :+:+: :+:+:  :+:                     //
//              +:+         +:+   +:+  +:+ +:+:+ +:+ +:+                      //
//             :#:        +#++:++#++: +#+  +:+  +#+ +#++:++#                  //
//            +#+   +#+# +#+     +#+ +#+       +#+ +#+                        //
//           #+#    #+# #+#     #+# #+#       #+# #+#                         //
//           ########  ###     ### ###       ### ##########                   //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <curses.h>

#include <vector>

class Entity;
struct Point;
struct Game;

enum e_gamestatus {
    STORY,
    MENU,
    PLAY,
    GAMEOVER
};
struct Region {
    Region(int _id) : id(_id) {};

    bool is_point_inside(Point const& p);
    std::vector<Region> get_adjacent_regions(Game const& world) const;

    int16_t id;
    int16_t width;
    int16_t height;
    int16_t origin_x;
    int16_t origin_y;
    std::vector<Entity> entities;
};

struct Game {
    WINDOW* main;
    WINDOW* hud;

    std::vector<Region> regions;
    Region get_region_by_id(int id) const;
    e_gamestatus status;
};
