#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace font_size {
enum {
  x36 = 0,
  x16,
  count,
};
}  // namespace font_size

class FontManager {
  TTF_Font *fonts_[font_size::count];

  inline void load(const char *path, unsigned int size,
                   const unsigned char font_size) {
    fonts_[font_size] = TTF_OpenFont(path, size);
    if (!fonts_[font_size]) {
      throw TTF_GetError();
    }
  }

 public:
  FontManager() noexcept {
    if (TTF_Init() == -1) {
      std::cerr << "error: " << TTF_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    try {
      load("./data/GenEiGothicP-Heavy.otf", 36, font_size::x36);
      load("./data/GenEiGothicP-Regular.otf", 16, font_size::x16);
    } catch (const char &e) {
      std::cerr << "error: " << e << '\n';
      exit(EXIT_FAILURE);
    }
  }

  inline TTF_Font *get(const unsigned char font_size) const noexcept {
    return fonts_[font_size];
  }

  ~FontManager() noexcept {
    for (const auto &font : fonts_) {
      TTF_CloseFont(font);
    }
    atexit(TTF_Quit);
  }
};

#endif
