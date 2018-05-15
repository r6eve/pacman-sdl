#ifndef FOOD_H
#define FOOD_H

#include "def_global.hpp"
#include "player.hpp"

class Food {
  // TODO: enum class
  // -1: no food, 0: counter food, 1: food, 2: get food
  char food_[block::count_y][block::count_x];

 public:
  Food() {}

  void init() noexcept;

  void draw(SDL_Surface *screen) noexcept;

  /**
   * Return true if all of the food are eaten, and false otherwise.
   */
  bool check_state(game_mode mode, Player &p1, Player &p2) noexcept;

  ~Food() {}
};

#endif
