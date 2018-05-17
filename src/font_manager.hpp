#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL/SDL_ttf.h>
#include <iostream>

namespace font_size {
enum {
  x36 = 0,
  x16,
  count,
};
}  // namespace font_size

class FontManager {
  TTF_Font *font_array_[font_size::count];

  inline void load(const char *path, unsigned int size,
                   const unsigned char font_size) {
    font_array_[font_size] = TTF_OpenFont(path, size);
    if (!font_array_[font_size]) {
      throw TTF_GetError();
    }
  }

 public:
  FontManager() noexcept {
    if (TTF_Init() != 0) {
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
    return font_array_[font_size];
  }

  ~FontManager() noexcept {
    for (unsigned int i = 0; i < font_size::count; ++i) {
      TTF_CloseFont(font_array_[i]);
    }
    atexit(TTF_Quit);
  }
};

#endif
