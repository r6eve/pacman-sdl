#include "wipe.hpp"
#include "def_global.hpp"

namespace wipe {

namespace {

int Wipe_variation;
int Wipe_count;
int Wipe_count_reach;

}  // namespace

void set_wipe_in() {
  Wipe_variation = -1;
  Wipe_count = SCREEN_HEIGHT / 10;
  Wipe_count_reach = 0;
}

void set_wipe_out() {
  Wipe_variation = 1;
  Wipe_count = 0;
  Wipe_count_reach = SCREEN_HEIGHT / 10;
}

bool update() {
  if (Wipe_count != Wipe_count_reach) {
    Wipe_count += Wipe_variation;
    return false;
  } else {
    return true;
  }
}

void draw(int end_x) {
  Uint32 black = 0x00000000;
  SDL_Rect dst;
  dst.x = 0;
  dst.w = end_x;
  dst.h = Wipe_count;
  const unsigned int dy = SCREEN_HEIGHT / 10;
  for (dst.y = 0; dst.y < SCREEN_HEIGHT; dst.y += dy) {
    SDL_FillRect(Screen, &dst, black);
  }
}

}  // namespace wipe
