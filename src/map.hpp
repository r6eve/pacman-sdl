#ifndef MAP_H
#define MAP_H

#include "def_global.hpp"

class Map {
  static unsigned int block_[block::count_y][block::count_x];

 public:
  static void init() noexcept;
  static unsigned int check_state(int x, int y) noexcept;
  static void draw() noexcept;
};

#endif
