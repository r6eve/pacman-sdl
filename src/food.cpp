#include "food.hpp"
#include "def_global.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "player.hpp"

namespace food {

namespace {

// TODO: enum class
// -1: no food, 0: counter food, 1: food, 2: get food
int food[NUM_BLOCK_Y][NUM_BLOCK_X];

}  // namespace

void init() {
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      food[y][x] = -1;
      if (map::check_state(x, y) == 0) {  // field where player can move
        food[y][x] = 1;
      } else if (map::check_state(x, y) == 4) {  // counter food
        food[y][x] = 0;
      }
    }
  }
}

void draw() {
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      if (food[y][x] == 2) {
        food[y][x] = -1;
      }
    }
  }

  SDL_Rect src = {0, 0, BLOCK_SIZE, BLOCK_SIZE};
  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      SDL_Rect dst;
      dst.x = BLOCK_SIZE * x;
      dst.y = BLOCK_SIZE * y;
      if (food[y][x] == 1) {  // food
        SDL_Surface *p_surface = image_manager::get_image("food");
        SDL_BlitSurface(p_surface, &src, Screen, &dst);
      } else if (food[y][x] == 0) {  // counter food
        SDL_Surface *p_surface = image_manager::get_image("food_counter");
        SDL_BlitSurface(p_surface, &src, Screen, &dst);
      }
    }
  }
}

bool check_state() {
  int x = player::get_player_1_block_x();
  int y = player::get_player_1_block_y();
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
  if (Game_mode == game_mode::battle) {
    int x = player::get_player_2_block_x();
    int y = player::get_player_2_block_y();
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

  return (rest_food == 0);
}

}  // namespace food
