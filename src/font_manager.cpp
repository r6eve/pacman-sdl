#include <SDL/SDL_ttf.h>
#include <iostream>
#include "font_manager.hpp"

using namespace std;

FontManager::FontManager() noexcept {
  if (TTF_Init() != 0) {
    cerr << "error: " << TTF_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  ttf_fonts_[0] = TTF_OpenFont("./data/GenEiGothicP-Heavy.otf", 36);
  ttf_fonts_[1] = TTF_OpenFont("./data/GenEiGothicP-Regular.otf", 16);
  if (!ttf_fonts_[0] || !ttf_fonts_[1]) {
    cerr << "error: " << TTF_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
}

TTF_Font *FontManager::get(const unsigned char size) const noexcept {
  if (size == 36) {
    return ttf_fonts_[0];
  }
  if (size == 16) {
    return ttf_fonts_[1];
  }

  return nullptr;
}

FontManager::~FontManager() noexcept {
  for (unsigned int i = 0; i < 2; ++i) {
    TTF_CloseFont(ttf_fonts_[i]);
  }
  atexit(TTF_Quit);
}
