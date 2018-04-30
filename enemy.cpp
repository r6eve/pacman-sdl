#include "enemy.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "main.hpp"
#include "main_chara.hpp"
#include "map.hpp"

static Enemy_data Enemy[NUM_ENEMY];

void init_enemy() {
  int start_block[NUM_ENEMY][2] = {{11, 12}, {12, 12}, {11, 11}, {12, 11}};
  for (int i = 0; i < NUM_ENEMY; ++i) {
    Enemy[i].pos_x = BLOCK_SIZE * start_block[i][0];
    Enemy[i].pos_y = BLOCK_SIZE * start_block[i][1];
    Enemy[i].block_x = Enemy[i].nextblock_x = start_block[i][0];
    Enemy[i].block_y = Enemy[i].nextblock_y = start_block[i][1];
    Enemy[i].dir = 2;
    Enemy[i].anime_count = 0;
    Enemy[i].anime_weight = 0;
  }
}

void update_enemy() {
  for (int i = 0; i < NUM_ENEMY; ++i) {
    ++Enemy[i].anime_weight;
    if (Enemy[i].anime_weight >= 8) {
      Enemy[i].anime_weight = 0;
      ++Enemy[i].anime_count;
      Enemy[i].anime_count %= 2;
    }
  }
}

void draw_enemy() {
  SDL_Rect src, dst;
  SDL_Surface *p_surface[5]; // TODO: 5
  p_surface[0] = get_img("akabei");
  p_surface[1] = get_img("pinky");
  p_surface[2] = get_img("aosuke");
  p_surface[3] = get_img("guzuta");
  p_surface[4] = get_img("mon_run");
  for (int i = 0; i < NUM_ENEMY; ++i) {
    switch (Enemy_state[i]) {
      case ENEMY_STATE_NORMAL:
        src.x = BLOCK_SIZE * Enemy[i].dir;
        src.y = BLOCK_SIZE * Enemy[i].anime_count;
        src.w = BLOCK_SIZE;
        src.h = BLOCK_SIZE;
        dst.x = Enemy[i].pos_x;
        dst.y = Enemy[i].pos_y;
        SDL_BlitSurface(p_surface[i], &src, Screen, &dst);
        break;
      case ENEMY_STATE_LOSE:
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

void mv_normal_enemy(int index) {
  int is_mving, dst_pos_x, dst_pos_y;

  switch (index) {
    case ENEMY_AKABEI:  // TODO: change moving algorithm for each enemies.
    case ENEMY_PINKY:
    case ENEMY_AOSUKE:
    case ENEMY_GUZUTA:
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
        if (check_map_state(Enemy[index].block_x, Enemy[index].block_y) == 2) {
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
          int front_block = check_map_state(front_block_x, front_block_y);
          if ((front_block == 3) || (front_block == 4) || (front_block == 5) ||
              (front_block == 6) || (front_block == 7) || (front_block == 8)) {
            front_block = 0;  // can move it
          }

          int left_block_x =
              Enemy[index].block_x + left_pos[Enemy[index].dir][0];
          int left_block_y =
              Enemy[index].block_y + left_pos[Enemy[index].dir][1];
          int left_block = check_map_state(left_block_x, left_block_y);
          if ((left_block == 3) || (left_block == 4) || (left_block == 5) ||
              (front_block == 8)) {
            left_block = 0;
          }

          int right_block_x =
              Enemy[index].block_x + right_pos[Enemy[index].dir][0];
          int right_block_y =
              Enemy[index].block_y + right_pos[Enemy[index].dir][1];
          int right_block = check_map_state(right_block_x, right_block_y);
          if ((right_block == 3) || (right_block == 4) || (right_block == 5) ||
              (front_block == 8)) {
            right_block = 0;
          }

          if (((rand() % 100) == 0) ||
              ((390 <= Power_chara_mode[0]) && (Power_chara_mode[0] <= 400)) ||
              ((390 <= Power_chara_mode[1]) && (Power_chara_mode[1] <= 400))) {
            if (check_map_state(
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

void mv_lose_enemy(int index) {
  if (!(Power_chara_mode[0] || Power_chara_mode[1])) {
    Enemy_state[index] = ENEMY_STATE_NORMAL;
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

void check_hit_enemy() {
  int main_chara_pos_x = get_main_chara_pos_x();
  int main_chara_pos_y = get_main_chara_pos_y();
  for (int i = 0; i < NUM_ENEMY; ++i) {
    int d = get_distance(main_chara_pos_x, main_chara_pos_y, Enemy[i].pos_x,
                         Enemy[i].pos_y);
    if (d < HIT_DISTANCE) {
      if (!Power_chara_mode[0]) {
        Choice_hit = true;
        Game_state = GAME_STATE_MISS;
      } else {
        if (Enemy_state[i] != ENEMY_STATE_LOSE) {
          Now_score[0] += 100;
        }
        Enemy_state[i] = ENEMY_STATE_LOSE;
      }
    }
  }
  if (Game_mode == GAME_MODE_BATTLE) {
    int rival_chara_pos_x = get_rival_chara_pos_x();
    int rival_chara_pos_y = get_rival_chara_pos_y();
    for (int i = 0; i < NUM_ENEMY; ++i) {
      int d = get_distance(rival_chara_pos_x, rival_chara_pos_y, Enemy[i].pos_x,
                           Enemy[i].pos_y);
      if (d < HIT_DISTANCE) {
        if (!Power_chara_mode[1]) {
          Choice_hit = false;
          Game_state = GAME_STATE_MISS;
        } else {
          if (Enemy_state[i] != ENEMY_STATE_LOSE) {
            Now_score[1] += 100;
          }
          Enemy_state[i] = ENEMY_STATE_LOSE;
        }
      }
    }
  }
}
