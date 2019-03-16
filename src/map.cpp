//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#include "map.hpp"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include "def_global.hpp"
#include "image_manager.hpp"

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
  const std::string block_src =
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

  // '.' : enemy can reach, '#' : enemy cannot reach
  const std::string enemy_reaches_block_p =
      "########################"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#.##.#####.##.#####.##.#"
      "#.##.#####.##.#####.##.#"
      "#......................#"
      "#.##.##.########.##.##.#"
      "#.##.##....##....##.##.#"
      "#....#####.##.#####....#"
      "####.#####.##.#####.####"
      "####.##..........##.####"
      "####.##.###.####.##.####"
      "........###.####........"
      "####.##.########.##.####"
      "####.##..........##.####"
      "####.##.########.##.####"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#..#................#..#"
      "##.#.##.########.##.#.##"
      "#....##....##....##....#"
      "#.########.##.########.#"
      "#......................#"
      "########################";

  // Max value which doesn't appear in the distance map.
  const unsigned int map_max_num = 10000;

  // BFS
  bool reachedp[block::count_y][block::count_x];
  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      home_distance_[y][x] = map_max_num;
      reachedp[y][x] = enemy_reaches_block_p[y * block::count_x + x] == '#';
    }
  }
  const Point losed = Point{11, 12};  // Point where a losed enemy is gone.
  const Point dirs[4] = {
      {0, -1}, {-1, 0}, {0, 1}, {1, 0}};  // down, left, up, right
  home_distance_[losed.y][losed.x] = 1;
  reachedp[losed.y][losed.x] = true;
  std::queue<Point> que;
  que.push(losed);
  while (!que.empty()) {
    const Point p = que.front();
    que.pop();
    const unsigned int d = home_distance_[p.y][p.x] + 1;
    for (const Point &dir : dirs) {
      const Point e = p + dir;
      if (e.x < 0 || e.x >= block::count_x || e.y < 0 || e.y >= block::count_y
          || reachedp[e.y][e.x]) {
        continue;
      }
      reachedp[e.y][e.x] = true;
      home_distance_[e.y][e.x] = d;
      que.push(e);
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
          const SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                                static_cast<Sint16>(block::size * y + block::size / 2),
                                block::size,
                                block::size};
          image_manager_->render_copy(*p_texture, src, dst);
        }
      }
    }
  }
  SDL_DestroyTexture(p_texture);
}
