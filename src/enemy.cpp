//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#include "enemy.hpp"
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"

void Enemy::move_normal_enemy(Enemy_data &enemy, const Map &map,
                              const Player &p1, const Player &p2) noexcept {
  // TODO: change moving algorithm for each enemy.
  const Point dst_pos = {enemy.next_block.x * block::size,
                         enemy.next_block.y * block::size};
  if (enemy.pos != dst_pos) {
    update();
    const unsigned int move_value = 2;
    if (dst_pos.x > enemy.pos.x) {
      enemy.pos.x += move_value;
    }
    if (dst_pos.y > enemy.pos.y) {
      enemy.pos.y += move_value;
    }
    if (dst_pos.x < enemy.pos.x) {
      enemy.pos.x -= move_value;
    }
    if (dst_pos.y < enemy.pos.y) {
      enemy.pos.y -= move_value;
    }
    return;
  }

  enemy.block = enemy.next_block;
  if (map.check_state(enemy.block) == map_state::enemy_house) {
    enemy.dir = 2;
    --enemy.next_block.y;
    return;
  }

  // { { down }, { left }, { up }, { right } }
  const Point front_pos[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
  const Point left_pos[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  const Point back_pos[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
  const Point right_pos[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

  const Point front_block = enemy.block + front_pos[enemy.dir];
  const map_state front_block_state = map.check_state(front_block);

  // TODO: make private function
  const bool move_to_front_block_p =
    front_block_state == map_state::init_p1_pos
    || front_block_state == map_state::init_p2_pos
    || front_block_state == map_state::food
    || front_block_state == map_state::counter_food
    || front_block_state == map_state::left_warp_pos
    || front_block_state == map_state::right_warp_pos
    || front_block_state == map_state::warp_street;

  const Point left_block = enemy.block + left_pos[enemy.dir];
  const map_state left_block_state = map.check_state(left_block);
  // TODO: make private function
  const bool move_to_left_block_p =
    left_block_state == map_state::food
    || left_block_state == map_state::init_p1_pos
    || left_block_state == map_state::init_p2_pos
    || left_block_state == map_state::counter_food;

  const Point right_block = enemy.block + right_pos[enemy.dir];
  const map_state right_block_state = map.check_state(right_block);
  // TODO: make private function
  const bool move_to_right_block_p =
    right_block_state == map_state::food
    || right_block_state == map_state::init_p1_pos
    || right_block_state == map_state::init_p2_pos
    || right_block_state == map_state::counter_food;

  // move back at random
  // TODO: make private function
  if (rand() % 100 == 0
      || (390 <= p1.get_power_mode() && p1.get_power_mode() <= 400)
      || (390 <= p2.get_power_mode() && p2.get_power_mode() <= 400)) {
    if (map.check_state(enemy.block + back_pos[enemy.dir])
        == map_state::enemy_house) {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    } else {
      enemy.next_block += back_pos[enemy.dir];
      enemy.dir += 2;
      enemy.dir %= 4;
    }
    return;
  }

  // move front/left/right at random
  // TODO: make private function
  if (move_to_front_block_p && move_to_left_block_p && move_to_right_block_p) {
    const unsigned int next = rand() % 3;
    if (next == 0) {
      enemy.next_block = front_block;
    } else if (next == 1) {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    } else {
      enemy.next_block = right_block;
      ++enemy.dir;
      enemy.dir %= 4;
    }
    return;
  }

  // move front/left at random
  if (move_to_front_block_p && move_to_left_block_p && !move_to_right_block_p) {
    const unsigned int next = rand() % 2;
    if (next == 0) {
      enemy.next_block = front_block;
    } else {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    }
    return;
  }

  // move front/right at random
  if (move_to_front_block_p && !move_to_left_block_p && move_to_right_block_p) {
    const unsigned int next = rand() % 2;
    if (next == 0) {
      enemy.next_block = front_block;
    } else {
      enemy.next_block = right_block;
      ++enemy.dir;
      enemy.dir %= 4;
    }
    return;
  }

  // move front
  if (move_to_front_block_p && !move_to_left_block_p
      && !move_to_right_block_p) {
    enemy.next_block = front_block;
    return;
  }

  // move left/right at random
  if (!move_to_front_block_p && move_to_left_block_p
      && move_to_right_block_p) {
    const unsigned int next = rand() % 2;
    if (next == 0) {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    } else {
      enemy.next_block = right_block;
      ++enemy.dir;
      enemy.dir %= 4;
    }
    return;
  }

  // move left
  if (!move_to_front_block_p && move_to_left_block_p
      && !move_to_right_block_p) {
    enemy.next_block = left_block;
    enemy.dir += 3;
    enemy.dir %= 4;
    return;
  }

  // move right
  if (!move_to_front_block_p && !move_to_left_block_p
      && move_to_right_block_p) {
    enemy.next_block = right_block;
    ++enemy.dir;
    enemy.dir %= 4;
    return;
  }

  // move back
  if (!move_to_front_block_p && !move_to_left_block_p &&
      !move_to_right_block_p) {
    enemy.next_block += back_pos[enemy.dir];
    enemy.dir += 2;
    enemy.dir %= 4;
    return;
  }

  // NOTREACHED
}

void Enemy::move_lose_enemy(Enemy_data &enemy, const Map &map, const Player &p1,
                            const Player &p2) noexcept {
  if (p1.get_power_mode() == 0 && p2.get_power_mode() == 0) {
    enemy.state = enemy_state::normal;
  }

  const Point dst_pos = {enemy.next_block.x * block::size,
                         enemy.next_block.y * block::size};
  if (enemy.pos != dst_pos) {
    update();
    const int move_value = 2;
    if (dst_pos.x > enemy.pos.x) {
      enemy.pos.x += move_value;
    }
    if (dst_pos.y > enemy.pos.y) {
      enemy.pos.y += move_value;
    }
    if (dst_pos.x < enemy.pos.x) {
      enemy.pos.x -= move_value;
    }
    if (dst_pos.y < enemy.pos.y) {
      enemy.pos.y -= move_value;
    }
    return;
  }

  enemy.block = enemy.next_block;
  const unsigned int now_value = map.get_home_distance(enemy.block);
  if (now_value
      > map.get_home_distance(Point{enemy.block.x, enemy.block.y - 1})) {
    --enemy.next_block.y;
    return;
  }

  if (now_value
      > map.get_home_distance(Point{enemy.block.x - 1, enemy.block.y})) {
    --enemy.next_block.x;
    return;
  }

  if (now_value
      > map.get_home_distance(Point{enemy.block.x + 1, enemy.block.y})) {
    ++enemy.next_block.x;
    return;
  }

  if (now_value
      > map.get_home_distance(Point{enemy.block.x, enemy.block.y + 1})) {
    ++enemy.next_block.y;
    return;
  }

  // enemy is in enemy's home
}

bool Enemy::check_hit_enemy(const game_mode mode, Player &p1,
                            Player &p2) noexcept {
  const unsigned int Hit_distance = block::size / 2;

  const Point pos = p1.get_pos();
  for (auto &enemy : enemies_) {
    const unsigned int d = pos.distance(enemy.pos);
    if (d < Hit_distance) {
      if (p1.get_power_mode() == 0) {
        p1.set_damaged(true);
        return true;
      }
      if (enemy.state == enemy_state::normal) {
        Mix_PlayChannel(se_type::beat_enemy,
                        mixer_manager_->get_se(se_type::beat_enemy), 0);
        p1.set_score(p1.get_score() + 100);
      }
      enemy.state = enemy_state::lose;
    }
  }

  if (mode == game_mode::battle) {
    const Point pos = p2.get_pos();
    for (auto &enemy : enemies_) {
      const unsigned int d = pos.distance(enemy.pos);
      if (d < Hit_distance) {
        if (p2.get_power_mode() == 0) {
          p2.set_damaged(true);
          return true;
        }
        if (enemy.state == enemy_state::normal) {
          Mix_PlayChannel(se_type::beat_enemy,
                          mixer_manager_->get_se(se_type::beat_enemy), 0);
          p2.set_score(p2.get_score() + 100);
        }
        enemy.state = enemy_state::lose;
      }
    }
  }

  return false;
}
