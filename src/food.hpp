#ifndef FOOD_H
#define FOOD_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"

class Food {
  enum class food_state {
    nothing,
    food,
    counter_food,
  };

  food_state food_[block::count_y][block::count_x];

 public:
  Food() {}

  void init(const Map &map) noexcept;

  void draw(SDL_Surface *screen, const ImageManager &image_manager) noexcept;

  /**
   * Return true if all of the food are eaten, and false otherwise.
   */
  bool check_state(const game_mode mode, const MixerManager &mixer_manager,
                   Player &p1, Player &p2) noexcept;

  ~Food() {}
};

#endif
