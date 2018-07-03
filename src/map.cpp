#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"

map_state char_to_map_state(const char c) {
  switch (c) {
    case '#':
      return map_state::block;
    case '.':
      return map_state::food;
    case 'C':
      return map_state::counter_food;
    case 'E':
      return map_state::enemy_house;
    case 'P':
      return map_state::init_p1_pos;
    case 'L':
      return map_state::left_warp_pos;
    case 'R':
      return map_state::right_warp_pos;
    case '=':
      return map_state::warp_street;
    default:
      std::cerr << "error: undefined character of map" << '\n';
      exit(EXIT_FAILURE);
  }
}

void Map::init(const game_mode mode) noexcept {
  std::string block_src =
      "########################"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#C##.#####.##.#####.##C#"
      "#.##.#####.##.#####.##.#"
      "#......................#"
      "#.##.##.########.##.##.#"
      "#.##.##....##....##.##.#"
      "#....#####.##.#####....#"
      "####.#####.##.#####.####"
      "####.##..........##.####"
      "####.##.###EE###.##.####"
      "L===....##EEEE##....===R"
      "####.##.########.##.####"
      "####.##..........##.####"
      "####.##.########.##.####"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#C.#.....P..........#.C#"
      "##.#.##.########.##.#.##"
      "#....##....##....##....#"
      "#.########.##.########.#"
      "#......................#"
      "########################";

  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      block_[y][x] = char_to_map_state(block_src[y * block::count_x + x]);
    }
  }

  if (mode == game_mode::battle) {
    block_[18][14] = map_state::init_p2_pos;
  }

  // 1: enemy house, 0: cannot move
  const unsigned int home_distance_src[block::count_y][block::count_x] = {
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
      home_distance_[y][x] = home_distance_src[y][x];
    }
  }
}

void Map::draw(const unsigned int game_level) const noexcept {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);

  SDL_Texture *p_texture = nullptr;
  const unsigned int mod = game_level % 4;
  if (mod == 1) {
    p_texture = image_manager_->get(image::bg);
  } else if (mod == 2) {
    p_texture = image_manager_->get(image::bg_red);
  } else if (mod == 3) {
    p_texture = image_manager_->get(image::bg_green);
  } else {
    p_texture = image_manager_->get(image::bg_blue);
  }

  {
    const SDL_Rect src[2] = {{0, 0, block::size, block::size},
                             {block::size, 0, block::size, block::size}};
    for (unsigned int y = 0; y < block::count_y; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        const SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                              static_cast<Sint16>(block::size * y), block::size,
                              block::size};
        switch (block_[y][x]) {
          case map_state::food:
          case map_state::counter_food:
          case map_state::enemy_house:
          case map_state::init_p1_pos:
          case map_state::init_p2_pos:
          case map_state::left_warp_pos:
          case map_state::right_warp_pos:
          case map_state::warp_street:
            image_manager_->render_copy(*p_texture, src[0], dst);
            break;
          case map_state::block:
            image_manager_->render_copy(*p_texture, src[1], dst);
            break;
        }
      }
    }
  }
  {
    const SDL_Rect src = {block::size, block::size, block::size,
                          block::size / 2};
    for (unsigned int y = 0; y < block::count_y - 1; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        const map_state block = block_[y][x];
        map_state mut_under_block = block_[y + 1][x];
        switch (mut_under_block) {
          case map_state::counter_food:
          case map_state::enemy_house:
          case map_state::init_p1_pos:
          case map_state::init_p2_pos:
          case map_state::left_warp_pos:
          case map_state::right_warp_pos:
          case map_state::warp_street:
            mut_under_block = map_state::food;
            break;
          default:
            // do nothing
            break;
        }
        const map_state under_block = mut_under_block;
        if ((block == map_state::block) && (under_block == map_state::food)) {
          const SDL_Rect dst = {
              static_cast<Sint16>(block::size * x),
              static_cast<Sint16>(block::size * y + block::size / 2),
              block::size, block::size};
          image_manager_->render_copy(*p_texture, src, dst);
        }
      }
    }
  }
  SDL_DestroyTexture(p_texture);
}
