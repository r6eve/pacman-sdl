//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

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
                   const unsigned char font_size) noexcept {
    fonts_[font_size] = TTF_OpenFont(path, size);
    if (!fonts_[font_size]) {
      std::cerr << "error: " << TTF_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
  }

 public:
  FontManager() noexcept {
    if (TTF_Init() == -1) {
      std::cerr << "error: " << TTF_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    load("./data/GenEiGothicP-Heavy.otf", 36, font_size::x36);
    load("./data/GenEiGothicP-Regular.otf", 16, font_size::x16);
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
