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

}  // namespace block

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

  Point add_x(const int x_arg) const noexcept {
    return Point{x + x_arg, y};
  }

  Point add_y(const int y_arg) const noexcept {
    return Point{x, y + y_arg};
  }
};

#endif
