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

// Keyboard and Joystick
namespace input_device {

enum {
  up = 0,
  down,
  left,
  right,
  x,
  c,
  b,  // Debug key: Enemys back home.
  space,

  // used by only Joystick
  button_2,
  button_3,
  button_4,
  button_5,
  button_6,
  button_7,
  button_8,
  button_9,

  start,
  select,
  count,
};

}  // namespace input_device

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

  bool operator==(const Point &rhs) { return ((x == rhs.x) && (y == rhs.y)); }

  bool operator!=(const Point &rhs) { return ((x != rhs.x) || (y != rhs.y)); }

  Point operator+(const Point &rhs) { return Point{x + rhs.x, y + rhs.y}; }

  void operator+=(const Point &rhs) {
    x += rhs.x;
    y += rhs.y;
  }

 public:
  unsigned int distance(const Point &rhs) noexcept {
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
GLOBAL int Power_chara_mode[2];
GLOBAL bool Press_key[2][input_device::count];
GLOBAL bool Edge_key[2][input_device::count];
GLOBAL unsigned int Home_way[block::count_y][block::count_x];
GLOBAL bool Choice_hit;  // true: player 1, false: player 2

#endif
