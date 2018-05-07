#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

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

namespace enemy_character {

enum {
  akabei = 0,
  pinky,
  aosuke,
  guzuta,
  count,
};

}  // namespace enemy_character

enum class enemy_state {
  normal,
  lose,
};

enum class game_mode {
  single,
  battle,
};

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL bool Debug_mode;
GLOBAL SDL_Surface *Screen;
GLOBAL unsigned int Now_score[2];  // 0: 1P, 1: 2P
GLOBAL int Game_level;
GLOBAL game_mode Game_mode;
GLOBAL enemy_state Enemy_state[enemy_character::count];
GLOBAL int Power_chara_mode[2];
GLOBAL bool Press_key[2][input_device::count];
GLOBAL bool Edge_key[2][input_device::count];
GLOBAL unsigned int Home_way[block::count_y][block::count_x];
GLOBAL Mix_Music *Music[5];  // TODO: enum class
GLOBAL Mix_Chunk *Se[4];     // TODO: enum class
GLOBAL bool Choice_hit;      // true: player 1, false: player 2

#endif
