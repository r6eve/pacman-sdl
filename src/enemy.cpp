#include "enemy.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "player.hpp"

void Enemy::move_normal_enemy(const unsigned int enemy_type, const Map &map,
                              const Player &p1, const Player &p2) noexcept {
  switch (enemy_type) {
    // TODO: change moving algorithm for each enemies.
    case enemy_character::akabei:
    case enemy_character::pinky:
    case enemy_character::aosuke:
    case enemy_character::guzuta: {
      const Point dst_pos = {enemies_[enemy_type].next_block.x * block::size,
                             enemies_[enemy_type].next_block.y * block::size};
      if (enemies_[enemy_type].pos != dst_pos) {
        update();
        const unsigned int move_value = 2;
        if (dst_pos.x > enemies_[enemy_type].pos.x) {
          enemies_[enemy_type].pos.x += move_value;
        }
        if (dst_pos.y > enemies_[enemy_type].pos.y) {
          enemies_[enemy_type].pos.y += move_value;
        }
        if (dst_pos.x < enemies_[enemy_type].pos.x) {
          enemies_[enemy_type].pos.x -= move_value;
        }
        if (dst_pos.y < enemies_[enemy_type].pos.y) {
          enemies_[enemy_type].pos.y -= move_value;
        }
        return;
      }

      enemies_[enemy_type].block = enemies_[enemy_type].next_block;
      if (map.check_state(enemies_[enemy_type].block) ==
          map_state::enemy_house) {
        enemies_[enemy_type].dir = 2;
        --enemies_[enemy_type].next_block.y;
        return;
      }

      // { { down }, { left }, { up }, { right } }
      const Point front_pos[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
      const Point left_pos[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
      const Point back_pos[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
      const Point right_pos[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

      const Point front_block =
          enemies_[enemy_type].block + front_pos[enemies_[enemy_type].dir];
      const map_state front_block_state = map.check_state(front_block);
      const bool move_to_front_block_p =
          ((front_block_state == map_state::init_p1_pos) ||
           (front_block_state == map_state::init_p2_pos) ||
           (front_block_state == map_state::food) ||
           (front_block_state == map_state::counter_food) ||
           (front_block_state == map_state::left_warp_pos) ||
           (front_block_state == map_state::right_warp_pos) ||
           (front_block_state == map_state::warp_street));

      const Point left_block =
          enemies_[enemy_type].block + left_pos[enemies_[enemy_type].dir];
      const map_state left_block_state = map.check_state(left_block);
      const bool move_to_left_block_p =
          ((left_block_state == map_state::food) ||
           (left_block_state == map_state::init_p1_pos) ||
           (left_block_state == map_state::init_p2_pos) ||
           (left_block_state == map_state::counter_food));

      const Point right_block =
          enemies_[enemy_type].block + right_pos[enemies_[enemy_type].dir];
      const map_state right_block_state = map.check_state(right_block);
      const bool move_to_right_block_p =
          ((right_block_state == map_state::food) ||
           (right_block_state == map_state::init_p1_pos) ||
           (right_block_state == map_state::init_p2_pos) ||
           (right_block_state == map_state::counter_food));

      // move back at random
      if (((rand() % 100) == 0) ||
          ((390 <= p1.get_power_mode()) && (p1.get_power_mode() <= 400)) ||
          ((390 <= p2.get_power_mode()) && (p2.get_power_mode() <= 400))) {
        if (map.check_state(enemies_[enemy_type].block +
                            back_pos[enemies_[enemy_type].dir]) ==
            map_state::enemy_house) {
          enemies_[enemy_type].next_block = left_block;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        } else {
          enemies_[enemy_type].next_block += back_pos[enemies_[enemy_type].dir];
          enemies_[enemy_type].dir += 2;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/left/right at random
      if (move_to_front_block_p && move_to_left_block_p &&
          move_to_right_block_p) {
        const unsigned int next = rand() % 3;
        if (next == 0) {
          enemies_[enemy_type].next_block = front_block;
        } else if (next == 1) {
          enemies_[enemy_type].next_block = left_block;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        } else {
          enemies_[enemy_type].next_block = right_block;
          ++enemies_[enemy_type].dir;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/left at random
      if (move_to_front_block_p && move_to_left_block_p &&
          !move_to_right_block_p) {
        const unsigned int next = rand() % 2;
        if (next == 0) {
          enemies_[enemy_type].next_block = front_block;
        } else {
          enemies_[enemy_type].next_block = left_block;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/right at random
      if (move_to_front_block_p && !move_to_left_block_p &&
          move_to_right_block_p) {
        const unsigned int next = rand() % 2;
        if (next == 0) {
          enemies_[enemy_type].next_block = front_block;
        } else {
          enemies_[enemy_type].next_block = right_block;
          ++enemies_[enemy_type].dir;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front
      if (move_to_front_block_p && !move_to_left_block_p &&
          !move_to_right_block_p) {
        enemies_[enemy_type].next_block = front_block;
        return;
      }

      // move left/right at random
      if (!move_to_front_block_p && move_to_left_block_p &&
          move_to_right_block_p) {
        const unsigned int next = rand() % 2;
        if (next == 0) {
          enemies_[enemy_type].next_block = left_block;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        } else {
          enemies_[enemy_type].next_block = right_block;
          ++enemies_[enemy_type].dir;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move left
      if (!move_to_front_block_p && move_to_left_block_p &&
          !move_to_right_block_p) {
        enemies_[enemy_type].next_block = left_block;
        enemies_[enemy_type].dir += 3;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // move right
      if (!move_to_front_block_p && !move_to_left_block_p &&
          move_to_right_block_p) {
        enemies_[enemy_type].next_block = right_block;
        ++enemies_[enemy_type].dir;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // move back
      if (!move_to_front_block_p && !move_to_left_block_p &&
          !move_to_right_block_p) {
        enemies_[enemy_type].next_block += back_pos[enemies_[enemy_type].dir];
        enemies_[enemy_type].dir += 2;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // NOTREACHED
      break;
    }
  }
}

void Enemy::move_lose_enemy(const unsigned int enemy_type, const Map &map,
                            const Player &p1, const Player &p2) noexcept {
  if ((p1.get_power_mode() == 0) && (p2.get_power_mode() == 0)) {
    enemies_[enemy_type].state = enemy_state::normal;
  }

  const Point dst_pos = {enemies_[enemy_type].next_block.x * block::size,
                         enemies_[enemy_type].next_block.y * block::size};
  if (enemies_[enemy_type].pos != dst_pos) {
    update();
    const int move_value = 2;
    if (dst_pos.x > enemies_[enemy_type].pos.x) {
      enemies_[enemy_type].pos.x += move_value;
    }
    if (dst_pos.y > enemies_[enemy_type].pos.y) {
      enemies_[enemy_type].pos.y += move_value;
    }
    if (dst_pos.x < enemies_[enemy_type].pos.x) {
      enemies_[enemy_type].pos.x -= move_value;
    }
    if (dst_pos.y < enemies_[enemy_type].pos.y) {
      enemies_[enemy_type].pos.y -= move_value;
    }
    return;
  }

  enemies_[enemy_type].block = enemies_[enemy_type].next_block;
  const unsigned int now_value =
      map.get_home_distance(enemies_[enemy_type].block);
  if (now_value >
      map.get_home_distance(Point{enemies_[enemy_type].block.x,
                                  enemies_[enemy_type].block.y - 1})) {
    --enemies_[enemy_type].next_block.y;
    return;
  }

  if (now_value > map.get_home_distance(Point{enemies_[enemy_type].block.x - 1,
                                              enemies_[enemy_type].block.y})) {
    --enemies_[enemy_type].next_block.x;
    return;
  }

  if (now_value > map.get_home_distance(Point{enemies_[enemy_type].block.x + 1,
                                              enemies_[enemy_type].block.y})) {
    ++enemies_[enemy_type].next_block.x;
    return;
  }

  if (now_value >
      map.get_home_distance(Point{enemies_[enemy_type].block.x,
                                  enemies_[enemy_type].block.y + 1})) {
    ++enemies_[enemy_type].next_block.y;
    return;
  }

  // enemy is in enemy's home
}

bool Enemy::check_hit_enemy(const game_mode mode, Player &p1, Player &p2) const
    noexcept {
  const unsigned int Hit_distance = block::size / 2;

  const Point pos = p1.get_pos();
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    const unsigned int d = pos.distance(enemies_[i].pos);
    if (d < Hit_distance) {
      if (p1.get_power_mode() == 0) {
        p1.set_damaged(true);
        return true;
      }
      if (enemies_[i].state == enemy_state::normal) {
        p1.set_score(p1.get_score() + 100);
      }
      enemies_[i].state = enemy_state::lose;
    }
  }

  if (mode == game_mode::battle) {
    const Point pos = p2.get_pos();
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      const unsigned int d = pos.distance(enemies_[i].pos);
      if (d < Hit_distance) {
        if (p2.get_power_mode() == 0) {
          p2.set_damaged(true);
          return true;
        }
        if (enemies_[i].state == enemy_state::normal) {
          p2.set_score(p2.get_score() + 100);
        }
        enemies_[i].state = enemy_state::lose;
      }
    }
  }

  return false;
}
