#include "wipe.hpp"
#include "def_global.hpp"

static int Wipe_variation;
static int Wipe_count;
static int Wipe_count_reach;

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

int update_wipe() {
  if (Wipe_count != Wipe_count_reach) {
    Wipe_count += Wipe_variation;
    return 0;
  } else {
    return 1;
  }
}

void draw_wipe(int end_x) {
  Uint32 black = 0x00000000;
  SDL_Rect dst;
  dst.x = 0;
  dst.w = end_x;
  dst.h = Wipe_count;
  for (dst.y = 0; dst.y < SCREEN_HEIGHT; dst.y += (SCREEN_HEIGHT / 10)) {
    SDL_FillRect(Screen, &dst, black);
  }
}
