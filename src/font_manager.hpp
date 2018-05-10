#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL/SDL_ttf.h>

class Font_manager {
  static TTF_Font *ttf_fonts_[2];

 public:
  /**
   * Call this function before using get() .
   */
  static void init();

  static TTF_Font *get(const unsigned char size) noexcept;

  /**
   * Call this function when program exits.
   */
  static void end() noexcept;
};

#endif
