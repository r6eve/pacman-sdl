#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define DEBUG

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32
#define BLOCK_SIZE 20
#define NUM_BLOCK_X 24
#define NUM_BLOCK_Y 24
#define OFFSET_X 480
#define FPS_MAX 60
#define ANIME_RATE 1
#define HIT_DISTANCE BLOCK_SIZE / 2
#define NUM_DEVICES 2

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
  bUTTON_2,
  bUTTON_3,
  bUTTON_4,
  bUTTON_5,
  bUTTON_6,
  bUTTON_7,
  bUTTON_8,
  bUTTON_9,

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

// game mode
enum class game_mode {
  single,
  battle,
};

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL SDL_Surface *Screen;
GLOBAL int Now_score[2];  // 0: 1P, 1: 2P
GLOBAL int Player_1_life;
GLOBAL int Player_2_life;
GLOBAL int Game_count;
GLOBAL int Game_level;
GLOBAL game_state Game_state;
GLOBAL game_mode Game_mode;
GLOBAL enemy_state Enemy_state[enemy_character::count];
GLOBAL int Power_chara_mode[2];
GLOBAL bool Press_key[2][input_device::count];
GLOBAL bool Edge_key[2][input_device::count];
GLOBAL int Home_way[NUM_BLOCK_Y][NUM_BLOCK_X];
GLOBAL bool Enemy_run_debug;
GLOBAL Mix_Music *Music[5];  // TODO: enum class
GLOBAL Mix_Chunk *Se[4];     // TODO: enum class
GLOBAL int Num_player;
GLOBAL bool Choice_hit;  // true: 1P, false: 2P

#endif
