#include "def_global.hpp"
#include "image_manager.hpp"
#include "input.hpp"
#include "map.hpp"
#include "player.hpp"

namespace player {

namespace {

// TODO: class
// TODO: add a member of life descriving Player_1_life/Player_2_life
typedef struct {
  int pos_x;
  int pos_y;
  int block_x;
  int block_y;
  int nextblock_x;
  int nextblock_y;
  int dir;          // 0: down, 1: left, 2: up, 3: right
  int anime_count;  // 0 or 1
  int anime_weight;
} Chara;

Chara Player_1;
Chara Player_2;

}

void init() {
  Player_1.pos_x = BLOCK_SIZE * 9;
  Player_1.pos_y = BLOCK_SIZE * 18;
  Player_1.nextblock_x = Player_1.block_x = 9;
  Player_1.nextblock_y = Player_1.block_y = 18;
  Player_1.dir = 1;
  Player_1.anime_count = 0;
  Player_1.anime_weight = 0;
  if (Game_mode == game_mode::battle) {
    Player_2.pos_x = BLOCK_SIZE * 14;
    Player_2.pos_y = BLOCK_SIZE * 18;
    Player_2.nextblock_x = Player_1.block_x = 14;
    Player_2.nextblock_y = Player_1.block_y = 18;
    Player_2.dir = 3;
    Player_2.anime_count = 0;
    Player_2.anime_weight = 0;
  }
}

void draw() {
  SDL_Surface *p_surface = image_manager::get_image("player1");
  SDL_Rect src;
  src.x = BLOCK_SIZE * Player_1.dir;
  src.y = BLOCK_SIZE * Player_1.anime_count;
  src.w = BLOCK_SIZE;
  src.h = BLOCK_SIZE;
  SDL_Rect dst;
  dst.x = Player_1.pos_x;
  dst.y = Player_1.pos_y;
  SDL_BlitSurface(p_surface, &src, Screen, &dst);
  if (Game_mode == game_mode::battle) {
    SDL_Surface *p_player_2 = image_manager::get_image("player2");
    SDL_Rect src_player_2;
    src_player_2.x = BLOCK_SIZE * Player_2.dir;
    src_player_2.y = BLOCK_SIZE * Player_2.anime_count;
    src_player_2.w = BLOCK_SIZE;
    src_player_2.h = BLOCK_SIZE;
    SDL_Rect dst_player_2;
    dst_player_2.x = Player_2.pos_x;
    dst_player_2.y = Player_2.pos_y;
    SDL_BlitSurface(p_player_2, &src_player_2, Screen, &dst_player_2);
  }
}

// TODO: reduce magic numbers
void move() {
  int is_mving;
  int dst_pos_x = Player_1.nextblock_x * BLOCK_SIZE;
  int dst_pos_y = Player_1.nextblock_y * BLOCK_SIZE;

  if ((Player_1.pos_x == dst_pos_x) && (Player_1.pos_y == dst_pos_y)) {
    is_mving = 0;
  } else {
    is_mving = 1;
  }

  if (is_mving) {
    ++Player_1.anime_weight;
    if (Player_1.anime_weight > 4) {
      Player_1.anime_count = 1 - Player_1.anime_count;
      Player_1.anime_weight = 0;
    }
    int mv_value = 2;
    if (dst_pos_x > Player_1.pos_x) {
      Player_1.pos_x += mv_value;
    } else if (dst_pos_x < Player_1.pos_x) {
      Player_1.pos_x -= mv_value;
    }
    if (dst_pos_y > Player_1.pos_y) {
      Player_1.pos_y += mv_value;
    } else if (dst_pos_y < Player_1.pos_y) {
      Player_1.pos_y -= mv_value;
    }
  } else {
    int dst_block_x = Player_1.nextblock_x;
    int dst_block_y = Player_1.nextblock_y;
    Player_1.block_x = dst_block_x;
    Player_1.block_y = dst_block_y;

    if (Press_key[0][input_device::down]) {
      Player_1.dir = 0;
      ++dst_block_y;
    } else if (Press_key[0][input_device::left]) {
      Player_1.dir = 1;
      --dst_block_x;
    } else if (Press_key[0][input_device::up]) {
      Player_1.dir = 2;
      --dst_block_y;
    } else if (Press_key[0][input_device::right]) {
      Player_1.dir = 3;
      ++dst_block_x;
    }

    if ((map::check_state(dst_block_x, dst_block_y) == 0) ||
        (map::check_state(dst_block_x, dst_block_y) == 3) ||
        (map::check_state(dst_block_x, dst_block_y) == 4) ||
        (map::check_state(dst_block_x, dst_block_y) == 5) ||
        (map::check_state(dst_block_x, dst_block_y) == 6) ||
        (map::check_state(dst_block_x + 1, dst_block_y) == 6) ||
        (map::check_state(dst_block_x, dst_block_y) == 7) ||
        (map::check_state(dst_block_x - 1, dst_block_y) == 7) ||
        (map::check_state(dst_block_x, dst_block_y) == 8)) {
      Player_1.nextblock_x = dst_block_x;
      Player_1.nextblock_y = dst_block_y;
    }

    if (map::check_state(dst_block_x + 2, dst_block_y) == 6) {
      Player_1.nextblock_x = NUM_BLOCK_X;
      Player_1.pos_x = BLOCK_SIZE * Player_1.nextblock_x;
    }

    if (map::check_state(dst_block_x - 2, dst_block_y) == 7) {
      Player_1.nextblock_x = -1;
      Player_1.pos_x = BLOCK_SIZE * Player_1.nextblock_x;
    }
  }

  if (Game_mode == game_mode::battle) {
    int is_mving;
    int dst_pos_x = Player_2.nextblock_x * BLOCK_SIZE;
    int dst_pos_y = Player_2.nextblock_y * BLOCK_SIZE;

    if ((Player_2.pos_x == dst_pos_x) && (Player_2.pos_y == dst_pos_y)) {
      is_mving = 0;
    } else {
      is_mving = 1;
    }

    if (is_mving) {
      ++Player_2.anime_weight;
      if (Player_2.anime_weight > 4) {
        Player_2.anime_count = 1 - Player_2.anime_count;
        Player_2.anime_weight = 0;
      }
      int mv_value = 2;
      if (dst_pos_x > Player_2.pos_x) {
        Player_2.pos_x += mv_value;
      } else if (dst_pos_x < Player_2.pos_x) {
        Player_2.pos_x -= mv_value;
      }

      if (dst_pos_y > Player_2.pos_y) {
        Player_2.pos_y += mv_value;
      } else if (dst_pos_y < Player_2.pos_y) {
        Player_2.pos_y -= mv_value;
      }
    } else {
      int dst_block_x = Player_2.nextblock_x;
      int dst_block_y = Player_2.nextblock_y;
      Player_2.block_x = dst_block_x;
      Player_2.block_y = dst_block_y;

      if (Press_key[1][input_device::down]) {
        Player_2.dir = 0;
        ++dst_block_y;
      } else if (Press_key[1][input_device::left]) {
        Player_2.dir = 1;
        --dst_block_x;
      } else if (Press_key[1][input_device::up]) {
        Player_2.dir = 2;
        --dst_block_y;
      } else if (Press_key[1][input_device::right]) {
        Player_2.dir = 3;
        ++dst_block_x;
      }

      if ((map::check_state(dst_block_x, dst_block_y) == 0) ||
          (map::check_state(dst_block_x, dst_block_y) == 3) ||
          (map::check_state(dst_block_x, dst_block_y) == 4) ||
          (map::check_state(dst_block_x, dst_block_y) == 5) ||
          (map::check_state(dst_block_x, dst_block_y) == 6) ||
          (map::check_state(dst_block_x + 1, dst_block_y) == 6) ||
          (map::check_state(dst_block_x, dst_block_y) == 7) ||
          (map::check_state(dst_block_x - 1, dst_block_y) == 7) ||
          (map::check_state(dst_block_x, dst_block_y) == 8)) {
        Player_2.nextblock_x = dst_block_x;
        Player_2.nextblock_y = dst_block_y;
      }

      if (map::check_state(dst_block_x + 2, dst_block_y) == 6) {
        Player_2.nextblock_x = NUM_BLOCK_X;
        Player_2.pos_x = BLOCK_SIZE * Player_2.nextblock_x;
      }

      if (map::check_state(dst_block_x - 2, dst_block_y) == 7) {
        Player_2.nextblock_x = -1;
        Player_2.pos_x = BLOCK_SIZE * Player_2.nextblock_x;
      }
    }
  }
}

void add_player_1_pos(int x, int y) {
  Player_1.pos_x += x;
  Player_1.pos_y += y;
}

void add_player_2_pos(int x, int y) {
  Player_2.pos_x += x;
  Player_2.pos_y += y;
}

int get_player_1_pos_x() { return Player_1.pos_x; }

int get_player_2_pos_x() { return Player_2.pos_x; }

int get_player_1_pos_y() { return Player_1.pos_y; }

int get_player_2_pos_y() { return Player_2.pos_y; }

int get_player_1_block_x() { return Player_1.block_x; }

int get_player_2_block_x() { return Player_2.block_x; }

int get_player_1_block_y() { return Player_1.block_y; }

int get_player_2_block_y() { return Player_2.block_y; }

}  // namespace player
