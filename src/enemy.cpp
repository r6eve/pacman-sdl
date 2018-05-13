#include "enemy.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "player.hpp"

const unsigned int Hit_distance = block::size / 2;

void Enemy::update() noexcept {
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    ++enemies_[i].anime_weight;
    if (enemies_[i].anime_weight >= 8) {
      enemies_[i].anime_weight = 0;
      ++enemies_[i].anime_count;
      enemies_[i].anime_count %= 2;
    }
  }
}

void Enemy::init() noexcept {
  const Point start_block[enemy_character::count] = {
      {11, 12}, {12, 12}, {11, 11}, {12, 11}};
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    enemies_[i].pos = {block::size * start_block[i].x,
                       block::size * start_block[i].y};
    enemies_[i].block = start_block[i];
    enemies_[i].next_block = start_block[i];
    enemies_[i].dir = 2;
    enemies_[i].anime_count = 0;
    enemies_[i].anime_weight = 0;
    enemies_[i].state = enemy_state::normal;
  }
}

void Enemy::draw() const noexcept {
  SDL_Surface *p_surface[5];  // TODO: 5
  p_surface[0] = Image_manager::get("akabei");
  p_surface[1] = Image_manager::get("pinky");
  p_surface[2] = Image_manager::get("aosuke");
  p_surface[3] = Image_manager::get("guzuta");
  p_surface[4] = Image_manager::get("mon_run");
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    SDL_Rect dst = {static_cast<Sint16>(enemies_[i].pos.x),
                    static_cast<Sint16>(enemies_[i].pos.y), 0, 0};
    switch (enemies_[i].state) {
      case enemy_state::normal: {
        SDL_Rect src = {
            static_cast<Sint16>(block::size * enemies_[i].dir),
            static_cast<Sint16>(block::size * enemies_[i].anime_count),
            block::size, block::size};
        SDL_BlitSurface(p_surface[i], &src, Screen, &dst);
        break;
      }
      case enemy_state::lose: {
        SDL_Rect src = {
            0, static_cast<Sint16>(block::size * enemies_[i].anime_count),
            block::size, block::size};
        SDL_BlitSurface(p_surface[4], &src, Screen, &dst);
        break;
      }
      default:
        // NOTREACHED
        break;
    }
  }
}

void Enemy::move(const bool debug_lose_enemy) noexcept {
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    if (debug_lose_enemy || (enemies_[i].state == enemy_state::lose)) {
      move_lose_enemy(i);
    } else {
      move_normal_enemy(i);
    }
  }
}

void Enemy::move_normal_enemy(const unsigned int enemy_type) noexcept {
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
      if (Map::check_state(enemies_[enemy_type].block) == 2) {
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
      unsigned int mut_front_block_state = Map::check_state(front_block);
      if ((mut_front_block_state == 3) || (mut_front_block_state == 4) ||
          (mut_front_block_state == 5) || (mut_front_block_state == 6) ||
          (mut_front_block_state == 7) || (mut_front_block_state == 8)) {
        mut_front_block_state = 0;  // can move it
      }
      const unsigned int front_block_state = mut_front_block_state;

      const Point left_block =
          enemies_[enemy_type].block + left_pos[enemies_[enemy_type].dir];
      unsigned int mut_left_block_state = Map::check_state(left_block);
      if ((mut_left_block_state == 3) || (mut_left_block_state == 4) ||
          (mut_left_block_state == 5) || (front_block_state == 8)) {
        mut_left_block_state = 0;
      }
      const unsigned int left_block_state = mut_left_block_state;

      const Point right_block =
          enemies_[enemy_type].block + right_pos[enemies_[enemy_type].dir];
      unsigned int mut_right_block_state = Map::check_state(right_block);
      if ((mut_right_block_state == 3) || (mut_right_block_state == 4) ||
          (mut_right_block_state == 5) || (front_block_state == 8)) {
        mut_right_block_state = 0;
      }
      const unsigned int right_block_state = mut_right_block_state;

      // move back at random
      if (((rand() % 100) == 0) ||
          ((390 <= Power_chara_mode[0]) && (Power_chara_mode[0] <= 400)) ||
          ((390 <= Power_chara_mode[1]) && (Power_chara_mode[1] <= 400))) {
        if (Map::check_state(enemies_[enemy_type].block +
                             back_pos[enemies_[enemy_type].dir]) == 2) {
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
      if ((front_block_state == 0) && (left_block_state == 0) &&
          (right_block_state == 0)) {
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
      if ((front_block_state == 0) && (left_block_state == 0) &&
          (right_block_state != 0)) {
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
      if ((front_block_state == 0) && (left_block_state != 0) &&
          (right_block_state == 0)) {
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
      if ((front_block_state == 0) && (left_block_state != 0) &&
          (right_block_state != 0)) {
        enemies_[enemy_type].next_block = front_block;
        return;
      }

      // move left/right at random
      if ((front_block_state != 0) && (left_block_state == 0) &&
          (right_block_state == 0)) {
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
      if ((front_block_state != 0) && (left_block_state == 0) &&
          (right_block_state != 0)) {
        enemies_[enemy_type].next_block = left_block;
        enemies_[enemy_type].dir += 3;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // move right
      if ((front_block_state != 0) && (left_block_state != 0) &&
          (right_block_state == 0)) {
        enemies_[enemy_type].next_block = right_block;
        ++enemies_[enemy_type].dir;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // move back
      if ((front_block_state != 0) && (left_block_state != 0) &&
          (right_block_state != 0)) {
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

void Enemy::move_lose_enemy(const unsigned int enemy_type) noexcept {
  if (!(Power_chara_mode[0] || Power_chara_mode[1])) {
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
      Home_way[enemies_[enemy_type].block.y][enemies_[enemy_type].block.x];
  if (now_value > Home_way[enemies_[enemy_type].block.y - 1]
                          [enemies_[enemy_type].block.x]) {
    --enemies_[enemy_type].next_block.y;
    return;
  }

  if (now_value > Home_way[enemies_[enemy_type].block.y]
                          [enemies_[enemy_type].block.x - 1]) {
    --enemies_[enemy_type].next_block.x;
    return;
  }

  if (now_value > Home_way[enemies_[enemy_type].block.y]
                          [enemies_[enemy_type].block.x + 1]) {
    ++enemies_[enemy_type].next_block.x;
    return;
  }

  if (now_value > Home_way[enemies_[enemy_type].block.y + 1]
                          [enemies_[enemy_type].block.x]) {
    ++enemies_[enemy_type].next_block.y;
    return;
  }

  // enemy is in enemy's home
}

bool Enemy::check_hit_enemy(Player &player1, Player &player2) const noexcept {
  Point pos = player1.get_pos();
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    const unsigned int d = pos.distance(enemies_[i].pos);
    if (d < Hit_distance) {
      if (!Power_chara_mode[0]) {
        player1.set_damaged(true);
        return true;
      }
      if (enemies_[i].state == enemy_state::normal) {
        player1.set_score(player1.get_score() + 100);
      }
      enemies_[i].state = enemy_state::lose;
    }
  }

  if (Game_mode == game_mode::battle) {
    Point pos = player2.get_pos();
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      const unsigned int d = pos.distance(enemies_[i].pos);
      if (d < Hit_distance) {
        if (!Power_chara_mode[1]) {
          player2.set_damaged(true);
          return true;
        }
        if (enemies_[i].state == enemy_state::normal) {
          player2.set_score(player2.get_score() + 100);
        }
        enemies_[i].state = enemy_state::lose;
      }
    }
  }

  return false;
}
