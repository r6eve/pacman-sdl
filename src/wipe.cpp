#include "wipe.hpp"
#include "def_global.hpp"

void Wipe::set_wipe_in() {
  wipe_variation_ = wipe_type::in;
  wipe_count_ = screen::height / 10;
  wipe_count_reach_ = 0;
}

void Wipe::set_wipe_out() {
  wipe_variation_ = wipe_type::out;
  wipe_count_ = 0;
  wipe_count_reach_ = screen::height / 10;
}

void Wipe::draw(unsigned int limit_x) {
  SDL_Rect dst = {0, 0, static_cast<Uint16>(limit_x),
                  static_cast<Uint16>(wipe_count_)};
  const unsigned int dy = screen::height / 10;
  for (dst.y = 0; dst.y < screen::height; dst.y += dy) {
    SDL_FillRect(Screen, &dst, 0x00000000);
  }
}

bool Wipe::update() {
  if (wipe_count_ == wipe_count_reach_) {
    return true;
  }

  switch (wipe_variation_) {
    case wipe_type::in:
      --wipe_count_;
      break;
    case wipe_type::out:
      ++wipe_count_;
      break;
    default:
      // NOTREACHED
      break;
  }
  return false;
}
