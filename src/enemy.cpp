#include "def_global.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "main.hpp"
#include "map.hpp"
#include "player.hpp"

namespace enemy {

static Enemy_data Enemy[enemy_character::count];

// private function
void update_enemy() {
  for (int i = 0; i < enemy_character::count; ++i) {
    ++Enemy[i].anime_weight;
    if (Enemy[i].anime_weight >= 8) {
      Enemy[i].anime_weight = 0;
      ++Enemy[i].anime_count;
      Enemy[i].anime_count %= 2;
    }
  }
}

void init_enemy() {
  int start_block[enemy_character::count][2] = {
      {11, 12}, {12, 12}, {11, 11}, {12, 11}};
  for (int i = 0; i < enemy_character::count; ++i) {
    Enemy[i].pos_x = BLOCK_SIZE * start_block[i][0];
    Enemy[i].pos_y = BLOCK_SIZE * start_block[i][1];
    Enemy[i].block_x = Enemy[i].nextblock_x = start_block[i][0];
    Enemy[i].block_y = Enemy[i].nextblock_y = start_block[i][1];
    Enemy[i].dir = 2;
    Enemy[i].anime_count = 0;
    Enemy[i].anime_weight = 0;
  }
}

void draw_enemy() {
  SDL_Rect src, dst;
  SDL_Surface *p_surface[5];  // TODO: 5
  p_surface[0] = image_manager::get_img("akabei");
  p_surface[1] = image_manager::get_img("pinky");
  p_surface[2] = image_manager::get_img("aosuke");
  p_surface[3] = image_manager::get_img("guzuta");
  p_surface[4] = image_manager::get_img("mon_run");
  for (int i = 0; i < enemy_character::count; ++i) {
    switch (Enemy_state[i]) {
      case enemy_state::normal:
        src.x = BLOCK_SIZE * Enemy[i].dir;
        src.y = BLOCK_SIZE * Enemy[i].anime_count;
        src.w = BLOCK_SIZE;
        src.h = BLOCK_SIZE;
        dst.x = Enemy[i].pos_x;
        dst.y = Enemy[i].pos_y;
        SDL_BlitSurface(p_surface[i], &src, Screen, &dst);
        break;
      case enemy_state::lose:
        src.x = 0;
        src.y = BLOCK_SIZE * Enemy[i].anime_count;
        src.w = BLOCK_SIZE;
        src.h = BLOCK_SIZE;
        dst.x = Enemy[i].pos_x;
        dst.y = Enemy[i].pos_y;
        SDL_BlitSurface(p_surface[4], &src, Screen, &dst);
        break;
      default:
        fprintf(stderr, "error: undefined enemy state.\n");
        break;
    }
  }
}

void move_normal_enemy(int index) {
  int is_mving, dst_pos_x, dst_pos_y;

  switch (index) {
    case enemy_character::akabei:  // TODO: change moving algorithm for each
                                   // enemies.
    case enemy_character::pinky:
    case enemy_character::aosuke:
    case enemy_character::guzuta:
      dst_pos_x = Enemy[index].nextblock_x * BLOCK_SIZE;
      dst_pos_y = Enemy[index].nextblock_y * BLOCK_SIZE;

      if ((Enemy[index].pos_x == dst_pos_x) &&
          (Enemy[index].pos_y == dst_pos_y)) {
        is_mving = 0;
      } else {
        is_mving = 1;
      }

      if (is_mving) {
        update_enemy();
        int mv_value = 2;
        if (dst_pos_x > Enemy[index].pos_x) {
          Enemy[index].pos_x += mv_value;
        }
        if (dst_pos_y > Enemy[index].pos_y) {
          Enemy[index].pos_y += mv_value;
        }
        if (dst_pos_x < Enemy[index].pos_x) {
          Enemy[index].pos_x -= mv_value;
        }
        if (dst_pos_y < Enemy[index].pos_y) {
          Enemy[index].pos_y -= mv_value;
        }
      } else {
        Enemy[index].block_x = Enemy[index].nextblock_x;
        Enemy[index].block_y = Enemy[index].nextblock_y;
        if (map::check_map_state(Enemy[index].block_x, Enemy[index].block_y) == 2) {
          Enemy[index].dir = 2;
          --Enemy[index].nextblock_y;
        } else {
          int front_pos[4][2] = {
              {0, 1},   // in down dir
              {-1, 0},  // in left dir
              {0, -1},  // in up dir
              {1, 0}    // in right dir
          };
          int left_pos[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
          int back_pos[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
          int right_pos[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

          int front_block_x =
              Enemy[index].block_x + front_pos[Enemy[index].dir][0];
          int front_block_y =
              Enemy[index].block_y + front_pos[Enemy[index].dir][1];
          int front_block = map::check_map_state(front_block_x, front_block_y);
          if ((front_block == 3) || (front_block == 4) || (front_block == 5) ||
              (front_block == 6) || (front_block == 7) || (front_block == 8)) {
            front_block = 0;  // can move it
          }

          int left_block_x =
              Enemy[index].block_x + left_pos[Enemy[index].dir][0];
          int left_block_y =
              Enemy[index].block_y + left_pos[Enemy[index].dir][1];
          int left_block = map::check_map_state(left_block_x, left_block_y);
          if ((left_block == 3) || (left_block == 4) || (left_block == 5) ||
              (front_block == 8)) {
            left_block = 0;
          }

          int right_block_x =
              Enemy[index].block_x + right_pos[Enemy[index].dir][0];
          int right_block_y =
              Enemy[index].block_y + right_pos[Enemy[index].dir][1];
          int right_block = map::check_map_state(right_block_x, right_block_y);
          if ((right_block == 3) || (right_block == 4) || (right_block == 5) ||
              (front_block == 8)) {
            right_block = 0;
          }

          if (((rand() % 100) == 0) ||
              ((390 <= Power_chara_mode[0]) && (Power_chara_mode[0] <= 400)) ||
              ((390 <= Power_chara_mode[1]) && (Power_chara_mode[1] <= 400))) {
            if (map::check_map_state(
                    Enemy[index].block_x + back_pos[Enemy[index].dir][0],
                    Enemy[index].block_y + back_pos[Enemy[index].dir][1]) ==
                2) {
              Enemy[index].nextblock_x = left_block_x;
              Enemy[index].nextblock_y = left_block_y;
              Enemy[index].dir += 3;
              Enemy[index].dir %= 4;
            } else {
              Enemy[index].nextblock_x += back_pos[Enemy[index].dir][0];
              Enemy[index].nextblock_y += back_pos[Enemy[index].dir][1];
              Enemy[index].dir += 2;
              Enemy[index].dir %= 4;
            }
          } else if ((front_block == 0) && (left_block == 0) &&
                     (right_block == 0)) {
            int next = rand() % 3;
            if (next == 0) {
              Enemy[index].nextblock_x = front_block_x;
              Enemy[index].nextblock_y = front_block_y;
            } else if (next == 1) {
              Enemy[index].nextblock_x = left_block_x;
              Enemy[index].nextblock_y = left_block_y;
              Enemy[index].dir += 3;
              Enemy[index].dir %= 4;
            } else {
              Enemy[index].nextblock_x = right_block_x;
              Enemy[index].nextblock_y = right_block_y;
              ++Enemy[index].dir;
              Enemy[index].dir %= 4;
            }
          } else if ((front_block == 0) && (left_block == 0) &&
                     (right_block != 0)) {
            int next = rand() % 2;
            if (next == 0) {
              Enemy[index].nextblock_x = front_block_x;
              Enemy[index].nextblock_y = front_block_y;
            } else {
              Enemy[index].nextblock_x = left_block_x;
              Enemy[index].nextblock_y = left_block_y;
              Enemy[index].dir += 3;
              Enemy[index].dir %= 4;
            }
          } else if ((front_block == 0) && (left_block != 0) &&
                     (right_block == 0)) {
            int next = rand() % 2;
            if (next == 0) {
              Enemy[index].nextblock_x = front_block_x;
              Enemy[index].nextblock_y = front_block_y;
            } else {
              Enemy[index].nextblock_x = right_block_x;
              Enemy[index].nextblock_y = right_block_y;
              ++Enemy[index].dir;
              Enemy[index].dir %= 4;
            }
          } else if ((front_block == 0) && (left_block != 0) &&
                     (right_block != 0)) {
            Enemy[index].nextblock_x = front_block_x;
            Enemy[index].nextblock_y = front_block_y;
          } else if ((front_block != 0) && (left_block == 0) &&
                     (right_block == 0)) {
            int next = rand() % 2;
            if (next == 0) {
              Enemy[index].nextblock_x = left_block_x;
              Enemy[index].nextblock_y = left_block_y;
              Enemy[index].dir += 3;
              Enemy[index].dir %= 4;
            } else {
              Enemy[index].nextblock_x = right_block_x;
              Enemy[index].nextblock_y = right_block_y;
              ++Enemy[index].dir;
              Enemy[index].dir %= 4;
            }
          } else if ((front_block != 0) && (left_block == 0) &&
                     (right_block != 0)) {
            Enemy[index].nextblock_x = left_block_x;
            Enemy[index].nextblock_y = left_block_y;
            Enemy[index].dir += 3;
            Enemy[index].dir %= 4;
          } else if ((front_block != 0) && (left_block != 0) &&
                     (right_block == 0)) {
            Enemy[index].nextblock_x = right_block_x;
            Enemy[index].nextblock_y = right_block_y;
            ++Enemy[index].dir;
            Enemy[index].dir %= 4;
          } else if ((front_block != 0) && (left_block != 0) &&
                     (right_block != 0)) {
            Enemy[index].nextblock_x += back_pos[Enemy[index].dir][0];
            Enemy[index].nextblock_y += back_pos[Enemy[index].dir][1];
            Enemy[index].dir += 2;
            Enemy[index].dir %= 4;
          }
        }
      }
      break;
  }
}

void move_lose_enemy(int index) {
  if (!(Power_chara_mode[0] || Power_chara_mode[1])) {
    Enemy_state[index] = enemy_state::normal;
  }
  int is_mving;
  int dst_pos_x = Enemy[index].nextblock_x * BLOCK_SIZE;
  int dst_pos_y = Enemy[index].nextblock_y * BLOCK_SIZE;

  if ((Enemy[index].pos_x == dst_pos_x) && (Enemy[index].pos_y == dst_pos_y)) {
    is_mving = 0;
  } else {
    is_mving = 1;
  }

  if (is_mving) {
    update_enemy();
    int mv_value = 2;
    if (dst_pos_x > Enemy[index].pos_x) {
      Enemy[index].pos_x += mv_value;
    }
    if (dst_pos_y > Enemy[index].pos_y) {
      Enemy[index].pos_y += mv_value;
    }
    if (dst_pos_x < Enemy[index].pos_x) {
      Enemy[index].pos_x -= mv_value;
    }
    if (dst_pos_y < Enemy[index].pos_y) {
      Enemy[index].pos_y -= mv_value;
    }
  } else {
    Enemy[index].block_x = Enemy[index].nextblock_x;
    Enemy[index].block_y = Enemy[index].nextblock_y;
    int now_value = Home_way[Enemy[index].block_y][Enemy[index].block_x];
    if (now_value > Home_way[Enemy[index].block_y - 1][Enemy[index].block_x]) {
      --Enemy[index].nextblock_y;
    } else if (now_value >
               Home_way[Enemy[index].block_y][Enemy[index].block_x - 1]) {
      --Enemy[index].nextblock_x;
    } else if (now_value >
               Home_way[Enemy[index].block_y][Enemy[index].block_x + 1]) {
      ++Enemy[index].nextblock_x;
    } else if (now_value >
               Home_way[Enemy[index].block_y + 1][Enemy[index].block_x]) {
      ++Enemy[index].nextblock_y;
    }
  }
}

bool check_hit_enemy() {
  int player_1_pos_x = player::get_player_1_pos_x();
  int player_1_pos_y = player::get_player_1_pos_y();
  for (int i = 0; i < enemy_character::count; ++i) {
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
    for (int i = 0; i < enemy_character::count; ++i) {
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
