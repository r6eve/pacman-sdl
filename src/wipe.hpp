#ifndef WIPE_H
#define WIPE_H

namespace wipe {

void set_wipe_in();

void set_wipe_out();

/**
 * Return true if wipe finishes.
 */
bool update();

void draw(unsigned int end_x);

}  // namespace wipe

#endif
