#include "enemy.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "player.hpp"
#include "util.hpp"

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
  const unsigned int start_block[enemy_character::count][2] = {
      {11, 12}, {12, 12}, {11, 11}, {12, 11}};
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    enemies_[i].pos_x = block::size * start_block[i][0];
    enemies_[i].pos_y = block::size * start_block[i][1];
    enemies_[i].block_x = enemies_[i].next_block_x = start_block[i][0];
    enemies_[i].block_y = enemies_[i].next_block_y = start_block[i][1];
    enemies_[i].dir = 2;
    enemies_[i].anime_count = 0;
    enemies_[i].anime_weight = 0;
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
    SDL_Rect dst = {static_cast<Sint16>(enemies_[i].pos_x),
                    static_cast<Sint16>(enemies_[i].pos_y), 0, 0};
    switch (Enemy_state[i]) {
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

void Enemy::move_normal_enemy(unsigned int enemy_type) noexcept {
  switch (enemy_type) {
    // TODO: change moving algorithm for each enemies.
    case enemy_character::akabei:
    case enemy_character::pinky:
    case enemy_character::aosuke:
    case enemy_character::guzuta: {
      const int dst_pos_x = enemies_[enemy_type].next_block_x * block::size;
      const int dst_pos_y = enemies_[enemy_type].next_block_y * block::size;
      if ((enemies_[enemy_type].pos_x != dst_pos_x) ||
          (enemies_[enemy_type].pos_y != dst_pos_y)) {
        update();
        const unsigned int move_value = 2;
        if (dst_pos_x > enemies_[enemy_type].pos_x) {
          enemies_[enemy_type].pos_x += move_value;
        }
        if (dst_pos_y > enemies_[enemy_type].pos_y) {
          enemies_[enemy_type].pos_y += move_value;
        }
        if (dst_pos_x < enemies_[enemy_type].pos_x) {
          enemies_[enemy_type].pos_x -= move_value;
        }
        if (dst_pos_y < enemies_[enemy_type].pos_y) {
          enemies_[enemy_type].pos_y -= move_value;
        }
        return;
      }

      enemies_[enemy_type].block_x = enemies_[enemy_type].next_block_x;
      enemies_[enemy_type].block_y = enemies_[enemy_type].next_block_y;
      if (Map::check_state(enemies_[enemy_type].block_x,
                           enemies_[enemy_type].block_y) == 2) {
        enemies_[enemy_type].dir = 2;
        --enemies_[enemy_type].next_block_y;
        return;
      }

      // { { down }, { left }, { up }, { right } }
      const int front_pos[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
      const int left_pos[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
      const int back_pos[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
      const int right_pos[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

      const int front_block_x =
          enemies_[enemy_type].block_x + front_pos[enemies_[enemy_type].dir][0];
      const int front_block_y =
          enemies_[enemy_type].block_y + front_pos[enemies_[enemy_type].dir][1];
      unsigned int mut_front_block =
          Map::check_state(front_block_x, front_block_y);
      if ((mut_front_block == 3) || (mut_front_block == 4) ||
          (mut_front_block == 5) || (mut_front_block == 6) ||
          (mut_front_block == 7) || (mut_front_block == 8)) {
        mut_front_block = 0;  // can move it
      }
      const unsigned int front_block = mut_front_block;

      const int left_block_x =
          enemies_[enemy_type].block_x + left_pos[enemies_[enemy_type].dir][0];
      const int left_block_y =
          enemies_[enemy_type].block_y + left_pos[enemies_[enemy_type].dir][1];
      unsigned int mut_left_block =
          Map::check_state(left_block_x, left_block_y);
      if ((mut_left_block == 3) || (mut_left_block == 4) ||
          (mut_left_block == 5) || (front_block == 8)) {
        mut_left_block = 0;
      }
      const unsigned int left_block = mut_left_block;

      const int right_block_x =
          enemies_[enemy_type].block_x + right_pos[enemies_[enemy_type].dir][0];
      const int right_block_y =
          enemies_[enemy_type].block_y + right_pos[enemies_[enemy_type].dir][1];
      unsigned int mut_right_block =
          Map::check_state(right_block_x, right_block_y);
      if ((mut_right_block == 3) || (mut_right_block == 4) ||
          (mut_right_block == 5) || (front_block == 8)) {
        mut_right_block = 0;
      }
      const unsigned int right_block = mut_right_block;

      // move back at random
      if (((rand() % 100) == 0) ||
          ((390 <= Power_chara_mode[0]) && (Power_chara_mode[0] <= 400)) ||
          ((390 <= Power_chara_mode[1]) && (Power_chara_mode[1] <= 400))) {
        if (Map::check_state(enemies_[enemy_type].block_x +
                                 back_pos[enemies_[enemy_type].dir][0],
                             enemies_[enemy_type].block_y +
                                 back_pos[enemies_[enemy_type].dir][1]) == 2) {
          enemies_[enemy_type].next_block_x = left_block_x;
          enemies_[enemy_type].next_block_y = left_block_y;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        } else {
          enemies_[enemy_type].next_block_x +=
              back_pos[enemies_[enemy_type].dir][0];
          enemies_[enemy_type].next_block_y +=
              back_pos[enemies_[enemy_type].dir][1];
          enemies_[enemy_type].dir += 2;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/left/right at random
      if ((front_block == 0) && (left_block == 0) && (right_block == 0)) {
        const unsigned int next = rand() % 3;
        if (next == 0) {
          enemies_[enemy_type].next_block_x = front_block_x;
          enemies_[enemy_type].next_block_y = front_block_y;
        } else if (next == 1) {
          enemies_[enemy_type].next_block_x = left_block_x;
          enemies_[enemy_type].next_block_y = left_block_y;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        } else {
          enemies_[enemy_type].next_block_x = right_block_x;
          enemies_[enemy_type].next_block_y = right_block_y;
          ++enemies_[enemy_type].dir;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/left at random
      if ((front_block == 0) && (left_block == 0) && (right_block != 0)) {
        const unsigned int next = rand() % 2;
        if (next == 0) {
          enemies_[enemy_type].next_block_x = front_block_x;
          enemies_[enemy_type].next_block_y = front_block_y;
        } else {
          enemies_[enemy_type].next_block_x = left_block_x;
          enemies_[enemy_type].next_block_y = left_block_y;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/right at random
      if ((front_block == 0) && (left_block != 0) && (right_block == 0)) {
        const unsigned int next = rand() % 2;
        if (next == 0) {
          enemies_[enemy_type].next_block_x = front_block_x;
          enemies_[enemy_type].next_block_y = front_block_y;
        } else {
          enemies_[enemy_type].next_block_x = right_block_x;
          enemies_[enemy_type].next_block_y = right_block_y;
          ++enemies_[enemy_type].dir;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move front
      if ((front_block == 0) && (left_block != 0) && (right_block != 0)) {
        enemies_[enemy_type].next_block_x = front_block_x;
        enemies_[enemy_type].next_block_y = front_block_y;
        return;
      }

      // move left/right at random
      if ((front_block != 0) && (left_block == 0) && (right_block == 0)) {
        const unsigned int next = rand() % 2;
        if (next == 0) {
          enemies_[enemy_type].next_block_x = left_block_x;
          enemies_[enemy_type].next_block_y = left_block_y;
          enemies_[enemy_type].dir += 3;
          enemies_[enemy_type].dir %= 4;
        } else {
          enemies_[enemy_type].next_block_x = right_block_x;
          enemies_[enemy_type].next_block_y = right_block_y;
          ++enemies_[enemy_type].dir;
          enemies_[enemy_type].dir %= 4;
        }
        return;
      }

      // move left
      if ((front_block != 0) && (left_block == 0) && (right_block != 0)) {
        enemies_[enemy_type].next_block_x = left_block_x;
        enemies_[enemy_type].next_block_y = left_block_y;
        enemies_[enemy_type].dir += 3;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // move right
      if ((front_block != 0) && (left_block != 0) && (right_block == 0)) {
        enemies_[enemy_type].next_block_x = right_block_x;
        enemies_[enemy_type].next_block_y = right_block_y;
        ++enemies_[enemy_type].dir;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // move back
      if ((front_block != 0) && (left_block != 0) && (right_block != 0)) {
        enemies_[enemy_type].next_block_x +=
            back_pos[enemies_[enemy_type].dir][0];
        enemies_[enemy_type].next_block_y +=
            back_pos[enemies_[enemy_type].dir][1];
        enemies_[enemy_type].dir += 2;
        enemies_[enemy_type].dir %= 4;
        return;
      }

      // NOTREACHED
      break;
    }
  }
}

void Enemy::move_lose_enemy(unsigned int enemy_type) noexcept {
  if (!(Power_chara_mode[0] || Power_chara_mode[1])) {
    Enemy_state[enemy_type] = enemy_state::normal;
  }

  const int dst_pos_x = enemies_[enemy_type].next_block_x * block::size;
  const int dst_pos_y = enemies_[enemy_type].next_block_y * block::size;
  if ((enemies_[enemy_type].pos_x != dst_pos_x) ||
      (enemies_[enemy_type].pos_y != dst_pos_y)) {
    update();
    const int move_value = 2;
    if (dst_pos_x > enemies_[enemy_type].pos_x) {
      enemies_[enemy_type].pos_x += move_value;
    }
    if (dst_pos_y > enemies_[enemy_type].pos_y) {
      enemies_[enemy_type].pos_y += move_value;
    }
    if (dst_pos_x < enemies_[enemy_type].pos_x) {
      enemies_[enemy_type].pos_x -= move_value;
    }
    if (dst_pos_y < enemies_[enemy_type].pos_y) {
      enemies_[enemy_type].pos_y -= move_value;
    }
    return;
  }

  enemies_[enemy_type].block_x = enemies_[enemy_type].next_block_x;
  enemies_[enemy_type].block_y = enemies_[enemy_type].next_block_y;
  const unsigned int now_value =
      Home_way[enemies_[enemy_type].block_y][enemies_[enemy_type].block_x];
  if (now_value > Home_way[enemies_[enemy_type].block_y - 1]
                          [enemies_[enemy_type].block_x]) {
    --enemies_[enemy_type].next_block_y;
    return;
  }

  if (now_value > Home_way[enemies_[enemy_type].block_y]
                          [enemies_[enemy_type].block_x - 1]) {
    --enemies_[enemy_type].next_block_x;
    return;
  }

  if (now_value > Home_way[enemies_[enemy_type].block_y]
                          [enemies_[enemy_type].block_x + 1]) {
    ++enemies_[enemy_type].next_block_x;
    return;
  }

  if (now_value > Home_way[enemies_[enemy_type].block_y + 1]
                          [enemies_[enemy_type].block_x]) {
    ++enemies_[enemy_type].next_block_y;
    return;
  }

  // enemy is in enemy's home
}

bool Enemy::check_hit_enemy(Player &player1, Player &player2) const noexcept {
  const int pos_x = player1.get_pos_x();
  const int pos_y = player1.get_pos_y();
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    const unsigned int d =
        util::get_distance(pos_x, pos_y, enemies_[i].pos_x, enemies_[i].pos_y);
    if (d < Hit_distance) {
      if (!Power_chara_mode[0]) {
        Choice_hit = true;
        return true;
      }
      if (Enemy_state[i] != enemy_state::lose) {
        player1.set_score(player1.get_score() + 100);
      }
      Enemy_state[i] = enemy_state::lose;
    }
  }

  if (Game_mode == game_mode::battle) {
    const int pos_x = player2.get_pos_x();
    const int pos_y = player2.get_pos_y();
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      const unsigned int d = util::get_distance(pos_x, pos_y, enemies_[i].pos_x,
                                                enemies_[i].pos_y);
      if (d < Hit_distance) {
        if (!Power_chara_mode[1]) {
          Choice_hit = false;
          return true;
        }
        if (Enemy_state[i] != enemy_state::lose) {
          player2.set_score(player2.get_score() + 100);
        }
        Enemy_state[i] = enemy_state::lose;
      }
    }
  }

  return false;
}
