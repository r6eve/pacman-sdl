#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "def_global.h"
#include "image_manager.h"
#include "main.h"
#include "map.h"

static int Block[NUM_BLOCK_Y][NUM_BLOCK_X];

void init_map() {
  // 0: can move, 1: cannot move, 2: enemy house
  // 3: player 1, 4: counter food, {5,6,7}: warp, 8: player 2
  int block_src[NUM_BLOCK_Y][NUM_BLOCK_X] = {
  // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 1
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 4, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 4, 1}, // 3
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // 4
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 5
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1}, // 6
    {1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1}, // 7
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // 9
    {1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1}, // 10
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // 11
    {6, 5, 5, 5, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0, 5, 5, 5, 7}, // 12
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // 13
    {1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1}, // 14
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // 15
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 16
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // 17
    {1, 4, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 4, 1}, // 18
    {1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1}, // 19
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1}, // 20
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1}, // 21
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 22
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 23
  };

  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      Block[y][x] = block_src[y][x];
    }
  }

  if (Game_mode == GAME_MODE_BATTLE) {
    Block[18][14] = 8;
  }

  // 1: enemy house, 0: cannot move
  int way_to_home[NUM_BLOCK_Y][NUM_BLOCK_X] = {
   // 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
    {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, //99
    {99, 22, 21, 20, 19, 20, 21, 22, 21, 20, 19, 99, 99, 20, 21, 22, 23, 22, 21, 20, 21, 22, 23, 99}, // 1
    {99, 21, 99, 99, 18, 99, 99, 99, 99, 99, 18, 99, 99, 19, 99, 99, 99, 99, 99, 19, 99, 99, 22, 99}, // 2
    {99, 20, 99, 99, 17, 99, 99, 99, 99, 99, 17, 99, 99, 18, 99, 99, 99, 99, 99, 18, 99, 99, 21, 99}, // 3
    {99, 19, 99, 99, 16, 99, 99, 99, 99, 99, 16, 99, 99, 17, 99, 99, 99, 99, 99, 17, 99, 99, 20, 99}, // 4
    {99, 18, 17, 16, 15, 14, 13, 12, 13, 14, 15, 16, 17, 16, 15, 14, 13, 14, 15, 16, 17, 18, 19, 99}, // 5
    {99, 19, 99, 99, 16, 99, 99, 11, 99, 99, 99, 99, 99, 99, 99, 99, 12, 99, 99, 17, 99, 99, 20, 99}, // 6
    {99, 20, 99, 99, 17, 99, 99, 10,  9,  8,  7, 99, 99,  8,  9, 10, 11, 99, 99, 18, 99, 99, 21, 99}, // 7
    {99, 19, 18, 17, 16, 99, 99, 99, 99, 99,  6, 99, 99,  7, 99, 99, 99, 99, 99, 17, 18, 19, 20, 99}, // 8
    {99, 99, 99, 99, 15, 99, 99, 99, 99, 99,  5, 99, 99,  6, 99, 99, 99, 99, 99, 16, 99, 99, 99, 99}, // 9
    {99, 99, 99, 99, 14, 99, 99,  7,  6,  5,  4,  3,  4,  5,  6,  7,  8, 99, 99, 15, 99, 99, 99, 99}, // 10
    {99, 99, 99, 99, 13, 99, 99,  8, 99, 99, 99,  2, 99, 99, 99, 99,  9, 99, 99, 14, 99, 99, 99, 99}, // 11
    {99, 99, 99, 99, 12, 11, 10,  9, 99, 99, 99,  1, 99, 99, 99, 99, 10, 11, 12, 13, 99, 99, 99, 99}, // 12
    {99, 99, 99, 99, 13, 99, 99, 10, 99, 99, 99, 99, 99, 99, 99, 99, 11, 99, 99, 14, 99, 99, 99, 99}, // 13
    {99, 99, 99, 99, 14, 99, 99, 11, 12, 13, 14, 15, 16, 15, 14, 13, 12, 99, 99, 15, 99, 99, 99, 99}, // 14
    {99, 99, 99, 99, 15, 99, 99, 12, 99, 99, 99, 99, 99, 99, 99, 99, 13, 99, 99, 16, 99, 99, 99, 99}, // 15
    {99, 19, 18, 17, 16, 15, 14, 13, 14, 15, 16, 99, 99, 17, 16, 15, 14, 15, 16, 17, 18, 19, 20, 99}, // 16
    {99, 20, 99, 99, 17, 99, 99, 99, 99, 99, 17, 99, 99, 18, 99, 99, 99, 99, 99, 18, 99, 99, 21, 99}, // 17
    {99, 21, 22, 99, 18, 19, 20, 21, 20, 19, 18, 19, 20, 19, 20, 21, 22, 21, 20, 19, 99, 23, 22, 99}, // 18
    {99, 99, 23, 99, 19, 99, 99, 22, 99, 99, 99, 99, 99, 99, 99, 99, 23, 99, 99, 20, 99, 24, 99, 99}, // 19
    {99, 23, 22, 21, 20, 99, 99, 23, 24, 25, 26, 99, 99, 27, 26, 25, 24, 99, 99, 21, 22, 23, 24, 99}, // 20
    {99, 24, 99, 99, 99, 99, 99, 99, 99, 99, 27, 99, 99, 28, 99, 99, 99, 99, 99, 99, 99, 99, 25, 99}, // 21
    {99, 25, 26, 27, 28, 29, 30, 31, 30, 29, 28, 29, 30, 29, 30, 31, 32, 31, 30, 29, 28, 27, 26, 99}, // 22
    {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 23
  };

  for (int y = 0; y < NUM_BLOCK_Y; ++y) {
    for (int x = 0; x < NUM_BLOCK_X; ++x) {
      Home_way[y][x] = way_to_home[y][x];
    }
  }
}

int check_map_state(int x, int y) { return Block[y][x]; }

void draw_map() {
  Uint32 black = 0x00000000;
  SDL_Rect dst = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_FillRect(Screen, &dst, black);

  SDL_Surface *p_surface = NULL;
  if ((Game_level % 4) == 1) {
    p_surface = get_img("bg");
  } else if ((Game_level % 4) == 2) {
    p_surface = get_img("bg_red");
  } else if ((Game_level % 4) == 3) {
    p_surface = get_img("bg_green");
  } else if ((Game_level % 4) == 0) {
    p_surface = get_img("bg_blue");
  }

  {
    SDL_Rect src[2];

    src[0].x = 0;
    src[1].x = BLOCK_SIZE;
    src[0].y = src[1].y = 0;
    src[0].w = src[1].w = BLOCK_SIZE;
    src[0].h = src[1].h = BLOCK_SIZE;
    for (int y = 0; y < NUM_BLOCK_Y; ++y) {
      for (int x = 0; x < NUM_BLOCK_X; ++x) {
        SDL_Rect dst;
        dst.x = BLOCK_SIZE * x;
        dst.y = BLOCK_SIZE * y;
        int block = Block[y][x];
        if ((block == 2) || (block == 3) || (block == 4) || (block == 5) ||
            (block == 6) || (block == 7) || (block == 8)) {
          block = 0;
        }
        SDL_BlitSurface(p_surface, &src[block], Screen, &dst);
      }
    }
  }
  {
    SDL_Rect src = {BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE / 2};
    for (int y = 0; y < NUM_BLOCK_Y - 1; ++y) {
      for (int x = 0; x < NUM_BLOCK_X; ++x) {
        int block = Block[y][x];
        int under_block = Block[y + 1][x];
        if ((under_block == 2) || (under_block == 3) || (under_block == 4) ||
            (under_block == 5) || (under_block == 6) || (under_block == 7) ||
            (under_block == 8)) {
          under_block = 0;
        }

        if ((block == 1) && (under_block == 0)) {
          SDL_Rect dst;
          dst.x = BLOCK_SIZE * x;
          dst.y = BLOCK_SIZE * y + BLOCK_SIZE / 2;
          SDL_BlitSurface(p_surface, &src, Screen, &dst);
        }
      }
    }
  }
}
