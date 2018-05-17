#ifndef MAP_H
#define MAP_H

#include "def_global.hpp"
#include "image_manager.hpp"

enum class map_state {
  block,
  food,
  counter_food,
  enemy_house,
  init_p1_pos,
  init_p2_pos,
  left_warp_pos,
  right_warp_pos,
  warp_street,
};

class Map {
  map_state block_[block::count_y][block::count_x];
  unsigned int home_distance_[block::count_y][block::count_x];

 public:
  Map() {}

  void init(const game_mode mode) noexcept;

  void draw(SDL_Surface *screen, const ImageManager &image_manager,
            const unsigned int game_level) const noexcept;

  map_state check_state(const Point &p) const noexcept;
  map_state check_state(const Point &&p) const noexcept;

  unsigned int get_home_distance(const Point &p) const noexcept;
  unsigned int get_home_distance(const Point &&p) const noexcept;

  ~Map() {}
};

#endif
