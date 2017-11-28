#include "food.h"
#include "def_global.h"
#include "enemy.h"
#include "image_manager.h"
#include "main_chara.h"
#include "map.h"

// -1: no food, 0: counter food, 1: food, 2: get food
static int food[NUM_BLOCK_Y][NUM_BLOCK_X];

void init_food() {
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      food[y][x] = -1;
      if (check_map_state(x, y) == 0) {
        food[y][x] = 1;
      } else if (check_map_state(x, y) == 4) {
        food[y][x] = 0;
      }
    }
  }
}

void update_food() {
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      if (food[y][x] == 2) {
        food[y][x] = -1;
      }
    }
  }
}

void draw_food() {
  SDL_Rect src = {0, 0, BLOCK_SIZE, BLOCK_SIZE};
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      SDL_Rect dst;
      dst.x = BLOCK_SIZE * x;
      dst.y = BLOCK_SIZE * y;
      if (food[y][x] == 1) {
        SDL_Surface *p_surface = get_img("food");
        SDL_BlitSurface(p_surface, &src, Screen, &dst);
      } else if (food[y][x] == 0) {
        SDL_Surface *p_surface = get_img("food_counter");
        SDL_BlitSurface(p_surface, &src, Screen, &dst);
      }
    }
  }
}

void check_food_state() {
  int x = get_main_chara_block_x();
  int y = get_main_chara_block_y();
  if (food[y][x] == 1) {
    Mix_PlayChannel(-1, Se[0], 0);
    ++food[y][x];
    Now_score[0] += 10;
  }
  if (food[y][x] == 0) {
    Power_chara_mode[0] = 400;
    Mix_PlayMusic(Music[0], -1);
    food[y][x] += 2;
  }
  if ((Power_chara_mode[0] == 0) && (Power_chara_mode[1] == 0)) {
    while (!Mix_FadeOutMusic(800) && Mix_PlayingMusic()) {
      ;
    }
  }
  if (Game_mode == GAME_MODE_BATTLE) {
    int x = get_rival_chara_block_x();
    int y = get_rival_chara_block_y();
    if (food[y][x] == 1) {
      Mix_PlayChannel(-1, Se[0], 0);
      ++food[y][x];
      Now_score[1] += 10;
    }
    if (food[y][x] == 0) {
      Power_chara_mode[1] = 400;
      Mix_PlayMusic(Music[1], -1);
      food[y][x] += 2;
    }
    if ((Power_chara_mode[0] == 0) && (Power_chara_mode[1] == 0)) {
      while (!Mix_FadeOutMusic(800) && Mix_PlayingMusic()) {
        ;
      }
    }
  }
  int rest_food = 0;
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      if ((food[y][x] == 0) || (food[y][x] == 1)) {
        ++rest_food;
      }
    }
  }
  if (rest_food == 0) {
    Game_state = GAME_STATE_CLEAR;
  }
}
