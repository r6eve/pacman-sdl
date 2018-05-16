#ifndef WIPE_H
#define WIPE_H

#include <SDL/SDL.h>

class Wipe {
  enum class wipe_type { in, out };

  wipe_type wipe_variation_;
  unsigned int wipe_count_;
  unsigned int wipe_count_reach_;

 public:
  Wipe() {}

  /**
   * Set wipe-in mode for drawing.
   */
  void set_wipe_in() noexcept;

  /**
   * Set wipe-out mode for drawing.
   */
  void set_wipe_out() noexcept;

  /**
   * Start drawing. Before execute this function, call set_wipe_in() or
   * set_wipe_out().
   *
   * `limit_x` is the limit for the x-axis to draw. It's bounded on closed
   * interval [0-screen::width] .
   */
  void draw(SDL_Surface *screen, const unsigned int limit_x) const noexcept;

  /**
   * Return true if wipe finishes.
   */
  bool update() noexcept;

  ~Wipe() {}
};

#endif
