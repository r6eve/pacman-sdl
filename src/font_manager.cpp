#include <SDL/SDL_ttf.h>

#include "font_manager.hpp"

TTF_Font *Font_manager::ttf_fonts_[2];

void Font_manager::init() {
  if (TTF_Init() != 0) {
    throw TTF_GetError();
  }

  ttf_fonts_[0] = TTF_OpenFont("./data/GenEiGothicP-Heavy.otf", 36);
  ttf_fonts_[1] = TTF_OpenFont("./data/GenEiGothicP-Regular.otf", 16);
  if (!ttf_fonts_[0] || !ttf_fonts_[1]) {
    throw TTF_GetError();
  }
}

TTF_Font *Font_manager::get(const unsigned char size) noexcept {
  if (size == 36) {
    return ttf_fonts_[0];
  }
  if (size == 16) {
    return ttf_fonts_[1];
  }

  return nullptr;
}

void Font_manager::end() noexcept {
  for (unsigned int i = 0; i < 2; ++i) {
    TTF_CloseFont(ttf_fonts_[i]);
  }
  atexit(TTF_Quit);
}
