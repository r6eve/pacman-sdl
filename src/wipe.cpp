#include "wipe.hpp"
#include "def_global.hpp"

namespace wipe {

namespace {

enum class wipe_type { in, out };

wipe_type Wipe_variation;
unsigned int Wipe_count;
unsigned int Wipe_count_reach;

}  // namespace

void set_wipe_in() {
  Wipe_variation = wipe_type::in;
  Wipe_count = screen::height / 10;
  Wipe_count_reach = 0;
}

void set_wipe_out() {
  Wipe_variation = wipe_type::out;
  Wipe_count = 0;
  Wipe_count_reach = screen::height / 10;
}

bool update() {
  if (Wipe_count == Wipe_count_reach) {
    return true;
  }

  switch (Wipe_variation) {
    case wipe_type::in:
      --Wipe_count;
      break;
    case wipe_type::out:
      ++Wipe_count;
      break;
    default:
      // NOTREACHED
      break;
  }
  return false;
}

void draw(unsigned int end_x) {
  SDL_Rect dst = {0, 0, static_cast<Uint16>(end_x),
                  static_cast<Uint16>(Wipe_count)};
  const unsigned int dy = screen::height / 10;
  for (dst.y = 0; dst.y < screen::height; dst.y += dy) {
    SDL_FillRect(Screen, &dst, 0x00000000);
  }
}

}  // namespace wipe
