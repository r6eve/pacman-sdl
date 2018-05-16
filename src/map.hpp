#ifndef MAP_H
#define MAP_H

#include "def_global.hpp"
#include "image_manager.hpp"

class Map {
  unsigned int block_[block::count_y][block::count_x];
  unsigned int home_distance_[block::count_y][block::count_x];

 public:
  Map() {}

  void init(game_mode mode) noexcept;

  void draw(SDL_Surface *screen, ImageManager &image,
            const unsigned int game_level) const noexcept;

  unsigned int check_state(const Point &p) const noexcept;
  unsigned int check_state(const Point &&p) const noexcept;

  unsigned int get_home_distance(const Point &p) const noexcept;
  unsigned int get_home_distance(const Point &&p) const noexcept;

  ~Map() {}
};

#endif
