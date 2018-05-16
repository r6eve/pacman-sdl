#ifndef MAP_H
#define MAP_H

#include "def_global.hpp"
#include "image_manager.hpp"

class Map {
  static unsigned int block_[block::count_y][block::count_x];
  static unsigned int home_way_[block::count_y][block::count_x];

 public:
  static void init(game_mode mode) noexcept;
  static void draw(SDL_Surface *screen, ImageManager &image,
                   const unsigned int game_level) noexcept;
  static unsigned int check_state(const Point &p) noexcept;
  static unsigned int check_state(const Point &&p) noexcept;
  static unsigned int get_home_distance(const Point &p) noexcept;
  static unsigned int get_home_distance(const Point &&p) noexcept;
};

#endif
