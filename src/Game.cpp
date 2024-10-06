#include "Game.hpp"
#include "Entity.hpp"
#include <cmath>
#include <stdexcept>

bool Region::is_point_inside(Point const& p) {
    return (p.x >= this->origin_x && p.x < this->origin_x + this->width &&
            p.y >= this->origin_y && p.y < this->origin_y + this->height);
}

std::vector<Region> Region::get_adjacent_regions(Game const& world) const {
    std::vector<Region> adjacent_regions;
    // for (const auto& region : world.regions) {
    //     if ((region.origin_x == this->origin_x &&
    //          abs(region.origin_y - this->origin_y) == this->height) ||
    //         (region.origin_y == this->origin_y &&
    //          abs(region.origin_x - this->origin_x) == this->width)) {
    //         adjacent_regions.push_back(region);
    //     }
    // }
    return adjacent_regions;
}

// Region Game::get_region_by_id(int id) const {
//     // for (const auto& region : this->regions) {
//     //     if (region.id == id) {
//     //         return region;
//     //     }
//     // }
//     // TODO: maybe remove this and return something else
//     throw std::runtime_error("Region with the given ID not found");
// }