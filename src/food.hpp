#ifndef FOOD_H
#define FOOD_H

namespace food {

void init_food();

void draw_food();

/**
 * Return true if all of the food are eaten, and false otherwise.
 */
bool check_food_state();

}  // namespace food

#endif
