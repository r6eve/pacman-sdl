#ifndef FOOD_H
#define FOOD_H

#include "def_global.hpp"

class Food {
  // TODO: enum class
  // -1: no food, 0: counter food, 1: food, 2: get food
  char food[block::count_y][block::count_x];

 public:
  Food() {}

  void init();

  void draw();

  /**
   * Return true if all of the food are eaten, and false otherwise.
   */
  bool check_state();

  ~Food() {}
};

#endif
