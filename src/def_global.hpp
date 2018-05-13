#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <math.h>

namespace screen {

enum {
  width = 640,
  height = 480,
  bpp = 32,
  offset_x = 480,
  max_fps = 60,
};

}  // namespace screen

namespace block {

enum {
  size = 20,
  count_x = 24,
  count_y = 24,
};
}

enum class game_state {
  title,
  start,
  clear,
  miss,
  playing,
  gameover,
  pause,
};

enum class game_mode {
  single,
  battle,
};

struct Point {
  int x;
  int y;

  bool operator==(const Point &rhs) const noexcept {
    return ((x == rhs.x) && (y == rhs.y));
  }

  bool operator!=(const Point &rhs) const noexcept {
    return ((x != rhs.x) || (y != rhs.y));
  }

  Point operator+(const Point &rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y};
  }

  void operator+=(const Point &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
  }

 public:
  unsigned int distance(const Point &rhs) const noexcept {
    return static_cast<unsigned int>(
        hypot(static_cast<double>(x - rhs.x), static_cast<double>(y - rhs.y)));
  }
};

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL SDL_Surface *Screen;
GLOBAL game_mode Game_mode;

#endif
