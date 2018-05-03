#include "enemy.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "main.hpp"
#include "map.hpp"
#include "player.hpp"

namespace enemy {

// TODO: use private class
typedef struct {
  int pos_x;
  int pos_y;
  int block_x;
  int block_y;
  int next_block_x;
  int next_block_y;
  int dir;          // TODO: enum class 0:down, 1:left, 2:up, 3:right
  int anime_count;  // 0 or 1
  int anime_weight;
} Enemy_data;

static Enemy_data Enemy[enemy_character::count];

// private function
void update() {
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    ++Enemy[i].anime_weight;
    if (Enemy[i].anime_weight >= 8) {
      Enemy[i].anime_weight = 0;
      ++Enemy[i].anime_count;
      Enemy[i].anime_count %= 2;
    }
  }
}

void init() {
  const unsigned int start_block[enemy_character::count][2] = {
      {11, 12}, {12, 12}, {11, 11}, {12, 11}};
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    Enemy[i].pos_x = BLOCK_SIZE * start_block[i][0];
    Enemy[i].pos_y = BLOCK_SIZE * start_block[i][1];
    Enemy[i].block_x = Enemy[i].next_block_x = start_block[i][0];
    Enemy[i].block_y = Enemy[i].next_block_y = start_block[i][1];
    Enemy[i].dir = 2;
    Enemy[i].anime_count = 0;
    Enemy[i].anime_weight = 0;
  }
}

void draw() {
  SDL_Surface *p_surface[5];  // TODO: 5
  p_surface[0] = image_manager::get_image("akabei");
  p_surface[1] = image_manager::get_image("pinky");
  p_surface[2] = image_manager::get_image("aosuke");
  p_surface[3] = image_manager::get_image("guzuta");
  p_surface[4] = image_manager::get_image("mon_run");
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    SDL_Rect dst = {static_cast<Sint16>(Enemy[i].pos_x),
                    static_cast<Sint16>(Enemy[i].pos_y), 0, 0};
    switch (Enemy_state[i]) {
      case enemy_state::normal: {
        SDL_Rect src = {static_cast<Sint16>(BLOCK_SIZE * Enemy[i].dir),
                        static_cast<Sint16>(BLOCK_SIZE * Enemy[i].anime_count),
                        BLOCK_SIZE, BLOCK_SIZE};
        SDL_BlitSurface(p_surface[i], &src, Screen, &dst);
        break;
      }
      case enemy_state::lose: {
        SDL_Rect src = {0,
                        static_cast<Sint16>(BLOCK_SIZE * Enemy[i].anime_count),
                        BLOCK_SIZE, BLOCK_SIZE};
        SDL_BlitSurface(p_surface[4], &src, Screen, &dst);
        break;
      }
      default:
        fprintf(stderr, "error: undefined enemy state.\n");
        break;
    }
  }
}

void move_normal_enemy(unsigned int enemy_type) {
  switch (enemy_type) {
    // TODO: change moving algorithm for each enemies.
    case enemy_character::akabei:
    case enemy_character::pinky:
    case enemy_character::aosuke:
    case enemy_character::guzuta: {
      const int dst_pos_x = Enemy[enemy_type].next_block_x * BLOCK_SIZE;
      const int dst_pos_y = Enemy[enemy_type].next_block_y * BLOCK_SIZE;
      if ((Enemy[enemy_type].pos_x != dst_pos_x) ||
          (Enemy[enemy_type].pos_y != dst_pos_y)) {
        update();
        const int move_value = 2;
        if (dst_pos_x > Enemy[enemy_type].pos_x) {
          Enemy[enemy_type].pos_x += move_value;
        }
        if (dst_pos_y > Enemy[enemy_type].pos_y) {
          Enemy[enemy_type].pos_y += move_value;
        }
        if (dst_pos_x < Enemy[enemy_type].pos_x) {
          Enemy[enemy_type].pos_x -= move_value;
        }
        if (dst_pos_y < Enemy[enemy_type].pos_y) {
          Enemy[enemy_type].pos_y -= move_value;
        }
        return;
      }

      Enemy[enemy_type].block_x = Enemy[enemy_type].next_block_x;
      Enemy[enemy_type].block_y = Enemy[enemy_type].next_block_y;
      if (map::check_state(Enemy[enemy_type].block_x, Enemy[enemy_type].block_y) == 2) {
        Enemy[enemy_type].dir = 2;
        --Enemy[enemy_type].next_block_y;
        return;
      }

      // { { down }, { left }, { up }, { right } }
      const int front_pos[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
      const int left_pos[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
      const int back_pos[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
      const int right_pos[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

      const int front_block_x =
          Enemy[enemy_type].block_x + front_pos[Enemy[enemy_type].dir][0];
      const int front_block_y =
          Enemy[enemy_type].block_y + front_pos[Enemy[enemy_type].dir][1];
      unsigned int front_block = map::check_state(front_block_x, front_block_y);
      if ((front_block == 3) || (front_block == 4) || (front_block == 5) ||
          (front_block == 6) || (front_block == 7) || (front_block == 8)) {
        front_block = 0;  // can move it
      }

      const int left_block_x =
          Enemy[enemy_type].block_x + left_pos[Enemy[enemy_type].dir][0];
      const int left_block_y =
          Enemy[enemy_type].block_y + left_pos[Enemy[enemy_type].dir][1];
      unsigned int left_block = map::check_state(left_block_x, left_block_y);
      if ((left_block == 3) || (left_block == 4) || (left_block == 5) ||
          (front_block == 8)) {
        left_block = 0;
      }

      const int right_block_x =
          Enemy[enemy_type].block_x + right_pos[Enemy[enemy_type].dir][0];
      const int right_block_y =
          Enemy[enemy_type].block_y + right_pos[Enemy[enemy_type].dir][1];
      unsigned int mut_right_block = map::check_state(right_block_x, right_block_y);
      if ((mut_right_block == 3) || (mut_right_block == 4) || (mut_right_block == 5) ||
          (front_block == 8)) {
        mut_right_block = 0;
      }
      const unsigned int right_block = mut_right_block;

      // move back at random
      if (((rand() % 100) == 0) ||
          ((390 <= Power_chara_mode[0]) && (Power_chara_mode[0] <= 400)) ||
          ((390 <= Power_chara_mode[1]) && (Power_chara_mode[1] <= 400))) {
        if (map::check_state(
                Enemy[enemy_type].block_x + back_pos[Enemy[enemy_type].dir][0],
                Enemy[enemy_type].block_y + back_pos[Enemy[enemy_type].dir][1]) == 2) {
          Enemy[enemy_type].next_block_x = left_block_x;
          Enemy[enemy_type].next_block_y = left_block_y;
          Enemy[enemy_type].dir += 3;
          Enemy[enemy_type].dir %= 4;
        } else {
          Enemy[enemy_type].next_block_x += back_pos[Enemy[enemy_type].dir][0];
          Enemy[enemy_type].next_block_y += back_pos[Enemy[enemy_type].dir][1];
          Enemy[enemy_type].dir += 2;
          Enemy[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/left/right at random
      if ((front_block == 0) && (left_block == 0) && (right_block == 0)) {
        int next = rand() % 3;
        if (next == 0) {
          Enemy[enemy_type].next_block_x = front_block_x;
          Enemy[enemy_type].next_block_y = front_block_y;
        } else if (next == 1) {
          Enemy[enemy_type].next_block_x = left_block_x;
          Enemy[enemy_type].next_block_y = left_block_y;
          Enemy[enemy_type].dir += 3;
          Enemy[enemy_type].dir %= 4;
        } else {
          Enemy[enemy_type].next_block_x = right_block_x;
          Enemy[enemy_type].next_block_y = right_block_y;
          ++Enemy[enemy_type].dir;
          Enemy[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/left at random
      if ((front_block == 0) && (left_block == 0) && (right_block != 0)) {
        int next = rand() % 2;
        if (next == 0) {
          Enemy[enemy_type].next_block_x = front_block_x;
          Enemy[enemy_type].next_block_y = front_block_y;
        } else {
          Enemy[enemy_type].next_block_x = left_block_x;
          Enemy[enemy_type].next_block_y = left_block_y;
          Enemy[enemy_type].dir += 3;
          Enemy[enemy_type].dir %= 4;
        }
        return;
      }

      // move front/right at random
      if ((front_block == 0) && (left_block != 0) && (right_block == 0)) {
        int next = rand() % 2;
        if (next == 0) {
          Enemy[enemy_type].next_block_x = front_block_x;
          Enemy[enemy_type].next_block_y = front_block_y;
        } else {
          Enemy[enemy_type].next_block_x = right_block_x;
          Enemy[enemy_type].next_block_y = right_block_y;
          ++Enemy[enemy_type].dir;
          Enemy[enemy_type].dir %= 4;
        }
        return;
      }

      // move front
      if ((front_block == 0) && (left_block != 0) && (right_block != 0)) {
        Enemy[enemy_type].next_block_x = front_block_x;
        Enemy[enemy_type].next_block_y = front_block_y;
        return;
      }

      // move left/right at random
      if ((front_block != 0) && (left_block == 0) && (right_block == 0)) {
        int next = rand() % 2;
        if (next == 0) {
          Enemy[enemy_type].next_block_x = left_block_x;
          Enemy[enemy_type].next_block_y = left_block_y;
          Enemy[enemy_type].dir += 3;
          Enemy[enemy_type].dir %= 4;
        } else {
          Enemy[enemy_type].next_block_x = right_block_x;
          Enemy[enemy_type].next_block_y = right_block_y;
          ++Enemy[enemy_type].dir;
          Enemy[enemy_type].dir %= 4;
        }
        return;
      }

      // move left
      if ((front_block != 0) && (left_block == 0) && (right_block != 0)) {
        Enemy[enemy_type].next_block_x = left_block_x;
        Enemy[enemy_type].next_block_y = left_block_y;
        Enemy[enemy_type].dir += 3;
        Enemy[enemy_type].dir %= 4;
        return;
      }

      // move right
      if ((front_block != 0) && (left_block != 0) && (right_block == 0)) {
        Enemy[enemy_type].next_block_x = right_block_x;
        Enemy[enemy_type].next_block_y = right_block_y;
        ++Enemy[enemy_type].dir;
        Enemy[enemy_type].dir %= 4;
        return;
      }

      // move back
      if ((front_block != 0) && (left_block != 0) && (right_block != 0)) {
        Enemy[enemy_type].next_block_x += back_pos[Enemy[enemy_type].dir][0];
        Enemy[enemy_type].next_block_y += back_pos[Enemy[enemy_type].dir][1];
        Enemy[enemy_type].dir += 2;
        Enemy[enemy_type].dir %= 4;
        return;
      }

      // NOTREACHED
      break;
    }
  }
}

void move_lose_enemy(unsigned int enemy_type) {
  if (!(Power_chara_mode[0] || Power_chara_mode[1])) {
    Enemy_state[enemy_type] = enemy_state::normal;
  }
  int is_moving;
  int dst_pos_x = Enemy[enemy_type].next_block_x * BLOCK_SIZE;
  int dst_pos_y = Enemy[enemy_type].next_block_y * BLOCK_SIZE;

  if ((Enemy[enemy_type].pos_x == dst_pos_x) && (Enemy[enemy_type].pos_y == dst_pos_y)) {
    is_moving = 0;
  } else {
    is_moving = 1;
  }

  if (is_moving) {
    update();
    int move_value = 2;
    if (dst_pos_x > Enemy[enemy_type].pos_x) {
      Enemy[enemy_type].pos_x += move_value;
    }
    if (dst_pos_y > Enemy[enemy_type].pos_y) {
      Enemy[enemy_type].pos_y += move_value;
    }
    if (dst_pos_x < Enemy[enemy_type].pos_x) {
      Enemy[enemy_type].pos_x -= move_value;
    }
    if (dst_pos_y < Enemy[enemy_type].pos_y) {
      Enemy[enemy_type].pos_y -= move_value;
    }
  } else {
    Enemy[enemy_type].block_x = Enemy[enemy_type].next_block_x;
    Enemy[enemy_type].block_y = Enemy[enemy_type].next_block_y;
    int now_value = Home_way[Enemy[enemy_type].block_y][Enemy[enemy_type].block_x];
    if (now_value > Home_way[Enemy[enemy_type].block_y - 1][Enemy[enemy_type].block_x]) {
      --Enemy[enemy_type].next_block_y;
    } else if (now_value >
               Home_way[Enemy[enemy_type].block_y][Enemy[enemy_type].block_x - 1]) {
      --Enemy[enemy_type].next_block_x;
    } else if (now_value >
               Home_way[Enemy[enemy_type].block_y][Enemy[enemy_type].block_x + 1]) {
      ++Enemy[enemy_type].next_block_x;
    } else if (now_value >
               Home_way[Enemy[enemy_type].block_y + 1][Enemy[enemy_type].block_x]) {
      ++Enemy[enemy_type].next_block_y;
    }
  }
}

bool check_hit_enemy() {
  int player_1_pos_x = player::get_player_1_pos_x();
  int player_1_pos_y = player::get_player_1_pos_y();
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    int d = get_distance(player_1_pos_x, player_1_pos_y, Enemy[i].pos_x,
                         Enemy[i].pos_y);
    if (d < HIT_DISTANCE) {
      if (!Power_chara_mode[0]) {
        Choice_hit = true;
        return true;
      }
      if (Enemy_state[i] != enemy_state::lose) {
        Now_score[0] += 100;
      }
      Enemy_state[i] = enemy_state::lose;
    }
  }

  if (Game_mode == game_mode::battle) {
    int player_2_pos_x = player::get_player_2_pos_x();
    int player_2_pos_y = player::get_player_2_pos_y();
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      int d = get_distance(player_2_pos_x, player_2_pos_y, Enemy[i].pos_x,
                           Enemy[i].pos_y);
      if (d < HIT_DISTANCE) {
        if (!Power_chara_mode[1]) {
          Choice_hit = false;
          return true;
        }
        if (Enemy_state[i] != enemy_state::lose) {
          Now_score[1] += 100;
        }
        Enemy_state[i] = enemy_state::lose;
      }
    }
  }

  return false;
}

}  // namespace enemy
