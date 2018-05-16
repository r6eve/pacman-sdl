#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL/SDL_ttf.h>

class FontManager {
  TTF_Font *ttf_fonts_[2];

 public:
  FontManager() noexcept;

  TTF_Font *get(const unsigned char size) const noexcept;

  ~FontManager() noexcept;
};

#endif
