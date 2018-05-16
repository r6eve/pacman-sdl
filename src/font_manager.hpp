#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL/SDL_ttf.h>

namespace font_size {
enum {
  x36 = 0,
  x16,
  count,
};
}  // namespace font_size

class FontManager {
  TTF_Font *font_array_[font_size::count];

  void load(const char *path, unsigned int size, const unsigned char font_size);

 public:
  FontManager() noexcept;

  TTF_Font *get(const unsigned char font_size) const noexcept;

  ~FontManager() noexcept;
};

#endif
