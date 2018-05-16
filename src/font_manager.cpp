#include <SDL/SDL_ttf.h>
#include <iostream>
#include "font_manager.hpp"

using namespace std;

void FontManager::load(const char *path, unsigned int size,
                       const unsigned char font_size) {
  TTF_Font *font = TTF_OpenFont(path, size);
  if (!font) {
    throw TTF_GetError();
  }
  font_array_[font_size] = font;
}

FontManager::FontManager() noexcept {
  if (TTF_Init() != 0) {
    cerr << "error: " << TTF_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  try {
    load("./data/GenEiGothicP-Heavy.otf", 36, font_size::x36);
    load("./data/GenEiGothicP-Regular.otf", 16, font_size::x16);
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    exit(EXIT_FAILURE);
  }
}

TTF_Font *FontManager::get(const unsigned char font_size) const noexcept {
  return font_array_[font_size];
}

FontManager::~FontManager() noexcept {
  for (unsigned int i = 0; i < font_size::count; ++i) {
    TTF_CloseFont(font_array_[i]);
  }
  atexit(TTF_Quit);
}
