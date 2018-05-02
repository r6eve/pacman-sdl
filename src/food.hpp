#ifndef FOOD_H
#define FOOD_H

namespace food {

void init();

void draw();

/**
 * Return true if all of the food are eaten, and false otherwise.
 */
bool check_state();

}  // namespace food

#endif
