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

// TODO: enum class
// keyboard
enum {
  PRESS_KEY_UP = 0,
  PRESS_KEY_DOWN,
  PRESS_KEY_LEFT,
  PRESS_KEY_RIGHT,
  PRESS_KEY_X,
  PRESS_KEY_C,
  PRESS_KEY_B,  // Debug key: Enemys back home.
  PRESS_KEY_SPACE,
  // for Joystick
  PRESS_KEY_BUTTON_2,
  PRESS_KEY_BUTTON_3,
  PRESS_KEY_BUTTON_4,
  PRESS_KEY_BUTTON_5,
  PRESS_KEY_BUTTON_6,
  PRESS_KEY_BUTTON_7,
  PRESS_KEY_BUTTON_8,
  PRESS_KEY_BUTTON_9,
  PRESS_KEY_START,
  PRESS_KEY_SELECT,
  NUM_PRESS_KEY,
};

// game state
enum {
  GAME_STATE_TITLE = 0,
  GAME_STATE_START,
  GAME_STATE_CLEAR,
  GAME_STATE_MISS,
  GAME_STATE_GAME,
  GAME_STATE_OVER,
  GAME_STATE_PAUSE,
  NUM_GAME_STATE,
};

// enemys
enum {
  ENEMY_AKABEI = 0,
  ENEMY_PINKY,
  ENEMY_AOSUKE,
  ENEMY_GUZUTA,
  NUM_ENEMY,
};

// enemy state
enum {
  ENEMY_STATE_NORMAL = 0,
  ENEMY_STATE_LOSE,
  NUM_ENEMY_STATE,
};

// game mode
enum {
  GAME_MODE_1P = 0,
  GAME_MODE_BATTLE,
  NUM_GAME_MODE,
};

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL SDL_Surface *Screen;
GLOBAL int Now_score[2];  // 0: 1P, 1: 2P
GLOBAL int Main_chara_life;
GLOBAL int Rival_chara_life;
GLOBAL int Game_count;
GLOBAL int Game_level;
GLOBAL int Game_state;
GLOBAL int Game_mode;
GLOBAL int Enemy_state[NUM_ENEMY];
GLOBAL int Power_chara_mode[2];
GLOBAL bool Press_key[2][NUM_PRESS_KEY];
GLOBAL bool Edge_key[2][NUM_PRESS_KEY];
GLOBAL int Home_way[NUM_BLOCK_Y][NUM_BLOCK_X];
GLOBAL bool Enemy_run_debug;
GLOBAL Mix_Music *Music[5]; // TODO: enum class
GLOBAL Mix_Chunk *Se[4]; // TODO: enum class
GLOBAL int Num_player;
GLOBAL bool Choice_hit;  // true: 1P, false: 2P

#endif
