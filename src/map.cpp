#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"

unsigned int Map::block_[block::count_y][block::count_x];
unsigned int Map::home_way_[block::count_y][block::count_x];

void Map::init(game_mode mode) noexcept {
  // TODO: use enum class
  // TODO: create other maps
  // 0: can move, 1: cannot move, 2: enemy's house
  // 3: player 1, 4: counter food, {5,6,7}: warp, 8: player 2
  unsigned int block_src[block::count_y][block::count_x] = {
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

  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      block_[y][x] = block_src[y][x];
    }
  }

  if (mode == game_mode::battle) {
    block_[18][14] = 8;
  }

  // 1: enemy house, 0: cannot move
  const unsigned int home_way_src[block::count_y][block::count_x] = {
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

  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      home_way_[y][x] = home_way_src[y][x];
    }
  }
}

// TODO: reduce magic numbers
void Map::draw(const unsigned int game_level) noexcept {
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_FillRect(Screen, &dst, 0x00000000);

  SDL_Surface *p_surface = nullptr;
  const unsigned int mod = game_level % 4;
  if (mod == 1) {
    p_surface = Image_manager::get("bg");
  } else if (mod == 2) {
    p_surface = Image_manager::get("bg_red");
  } else if (mod == 3) {
    p_surface = Image_manager::get("bg_green");
  } else {
    p_surface = Image_manager::get("bg_blue");
  }

  {
    SDL_Rect src[2] = {{0, 0, block::size, block::size},
                       {block::size, 0, block::size, block::size}};
    for (unsigned int y = 0; y < block::count_y; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                        static_cast<Sint16>(block::size * y), 0, 0};
        int mut_block = block_[y][x];
        if ((mut_block == 2) || (mut_block == 3) || (mut_block == 4) ||
            (mut_block == 5) || (mut_block == 6) || (mut_block == 7) ||
            (mut_block == 8)) {
          mut_block = 0;
        }
        const int block = mut_block;
        SDL_BlitSurface(p_surface, &src[block], Screen, &dst);
      }
    }
  }
  {
    SDL_Rect src = {block::size, block::size, block::size, block::size / 2};
    for (unsigned int y = 0; y < block::count_y - 1; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        const int block = block_[y][x];
        int mut_under_block = block_[y + 1][x];
        if ((mut_under_block == 2) || (mut_under_block == 3) ||
            (mut_under_block == 4) || (mut_under_block == 5) ||
            (mut_under_block == 6) || (mut_under_block == 7) ||
            (mut_under_block == 8)) {
          mut_under_block = 0;
        }
        const int under_block = mut_under_block;

        if ((block == 1) && (under_block == 0)) {
          SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                          static_cast<Sint16>(block::size * y + block::size / 2),
                          0, 0};
          SDL_BlitSurface(p_surface, &src, Screen, &dst);
        }
      }
    }
  }
}

// Don't modify parameter type as unsigned int
// TODO: Why is the parameter of x=-1 and y=12 OK?
// Cf. https://ideone.com/u1QKTJ
unsigned int Map::check_state(const Point &p) noexcept {
  return block_[p.y][p.x];
}

unsigned int Map::check_state(const Point &&p) noexcept {
  return block_[p.y][p.x];
}

unsigned int Map::get_home_distance(const Point &p) noexcept {
  return home_way_[p.y][p.x];
}

unsigned int Map::get_home_distance(const Point &&p) noexcept {
  return get_home_distance(p);
}
