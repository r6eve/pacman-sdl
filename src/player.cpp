#include "player.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input.hpp"
#include "map.hpp"

namespace player {

namespace {

// TODO: class
typedef struct {
  int pos_x;
  int pos_y;
  int block_x;
  int block_y;
  int next_block_x;
  int next_block_y;
  unsigned char dir;           // 0: down, 1: left, 2: up, 3: right
  unsigned char anime_count;   // 0 or 1
  unsigned char anime_weight;  // max value is 4
  int life;
} Player;

Player Player_1;
Player Player_2;

}  // namespace

void init() {
  Player_1.pos_x = block::size * 9;
  Player_1.pos_y = block::size * 18;
  Player_1.next_block_x = Player_1.block_x = 9;
  Player_1.next_block_y = Player_1.block_y = 18;
  Player_1.dir = 1;
  Player_1.anime_count = 0;
  Player_1.anime_weight = 0;
  if (Game_mode == game_mode::battle) {
    Player_2.pos_x = block::size * 14;
    Player_2.pos_y = block::size * 18;
    Player_2.next_block_x = Player_1.block_x = 14;
    Player_2.next_block_y = Player_1.block_y = 18;
    Player_2.dir = 3;
    Player_2.anime_count = 0;
    Player_2.anime_weight = 0;
  }
}

void draw() {
  SDL_Surface *p_surface = Image_manager::get("player1");
  SDL_Rect src;
  src.x = block::size * Player_1.dir;
  src.y = block::size * Player_1.anime_count;
  src.w = block::size;
  src.h = block::size;
  SDL_Rect dst;
  dst.x = Player_1.pos_x;
  dst.y = Player_1.pos_y;
  SDL_BlitSurface(p_surface, &src, Screen, &dst);
  if (Game_mode == game_mode::battle) {
    SDL_Surface *p_player_2 = Image_manager::get("player2");
    SDL_Rect src_player_2;
    src_player_2.x = block::size * Player_2.dir;
    src_player_2.y = block::size * Player_2.anime_count;
    src_player_2.w = block::size;
    src_player_2.h = block::size;
    SDL_Rect dst_player_2;
    dst_player_2.x = Player_2.pos_x;
    dst_player_2.y = Player_2.pos_y;
    SDL_BlitSurface(p_player_2, &src_player_2, Screen, &dst_player_2);
  }
}

// TODO: reduce magic numbers
void move() {
  const int dst_pos_x = Player_1.next_block_x * block::size;
  const int dst_pos_y = Player_1.next_block_y * block::size;

  if ((Player_1.pos_x != dst_pos_x) || (Player_1.pos_y != dst_pos_y)) {
    ++Player_1.anime_weight;
    if (Player_1.anime_weight > 4) {
      Player_1.anime_count = 1 - Player_1.anime_count;
      Player_1.anime_weight = 0;
    }
    const unsigned int move_value = 2;
    if (dst_pos_x > Player_1.pos_x) {
      Player_1.pos_x += move_value;
    }
    if (dst_pos_y > Player_1.pos_y) {
      Player_1.pos_y += move_value;
    }
    if (dst_pos_x < Player_1.pos_x) {
      Player_1.pos_x -= move_value;
    }
    if (dst_pos_y < Player_1.pos_y) {
      Player_1.pos_y -= move_value;
    }
  } else {
    Player_1.block_x = Player_1.next_block_x;
    Player_1.block_y = Player_1.next_block_y;

    // 同時押しの場合，優先順位は
    // Down > Left > Up > Right
    int mut_dst_block_x = Player_1.next_block_x;
    int mut_dst_block_y = Player_1.next_block_y;
    if (Press_key[0][input_device::down]) {
      Player_1.dir = 0;
      ++mut_dst_block_y;
    } else if (Press_key[0][input_device::left]) {
      Player_1.dir = 1;
      --mut_dst_block_x;
    } else if (Press_key[0][input_device::up]) {
      Player_1.dir = 2;
      --mut_dst_block_y;
    } else if (Press_key[0][input_device::right]) {
      Player_1.dir = 3;
      ++mut_dst_block_x;
    }
    const int dst_block_x = mut_dst_block_x;
    const int dst_block_y = mut_dst_block_y;

    const unsigned int dst_block_state =
        Map::check_state(dst_block_x, dst_block_y);
    const unsigned int dst_right_block_state =
        Map::check_state(dst_block_x + 1, dst_block_y);
    const unsigned int dst_left_block_state =
        Map::check_state(dst_block_x - 1, dst_block_y);
    if ((dst_block_state == 0) || (dst_block_state == 3) ||
        (dst_block_state == 4) || (dst_block_state == 5) ||
        (dst_block_state == 6) || (dst_right_block_state == 6) ||
        (dst_block_state == 7) || (dst_left_block_state == 7) ||
        (dst_block_state == 8)) {
      Player_1.next_block_x = dst_block_x;
      Player_1.next_block_y = dst_block_y;
    }

    // Circle corner
    if (Map::check_state(dst_block_x + 2, dst_block_y) == 6) {
      Player_1.next_block_x = block::count_x;
      Player_1.pos_x = block::size * Player_1.next_block_x;
    }
    if (Map::check_state(dst_block_x - 2, dst_block_y) == 7) {
      Player_1.next_block_x = -1;
      Player_1.pos_x = block::size * Player_1.next_block_x;
    }
  }

  if (Game_mode == game_mode::battle) {
    const int dst_pos_x = Player_2.next_block_x * block::size;
    const int dst_pos_y = Player_2.next_block_y * block::size;

    if ((Player_2.pos_x != dst_pos_x) || (Player_2.pos_y != dst_pos_y)) {
      ++Player_2.anime_weight;
      if (Player_2.anime_weight > 4) {
        Player_2.anime_count = 1 - Player_2.anime_count;
        Player_2.anime_weight = 0;
      }
      const unsigned int move_value = 2;
      if (dst_pos_x > Player_2.pos_x) {
        Player_2.pos_x += move_value;
      }
      if (dst_pos_y > Player_2.pos_y) {
        Player_2.pos_y += move_value;
      }
      if (dst_pos_x < Player_2.pos_x) {
        Player_2.pos_x -= move_value;
      }
      if (dst_pos_y < Player_2.pos_y) {
        Player_2.pos_y -= move_value;
      }
    } else {
      Player_2.block_x = Player_2.next_block_x;
      Player_2.block_y = Player_2.next_block_y;

      // 同時押しの場合，優先順位は
      // Down > Left > Up > Right
      int mut_dst_block_x = Player_2.next_block_x;
      int mut_dst_block_y = Player_2.next_block_y;
      if (Press_key[1][input_device::down]) {
        Player_2.dir = 0;
        ++mut_dst_block_y;
      } else if (Press_key[1][input_device::left]) {
        Player_2.dir = 1;
        --mut_dst_block_x;
      } else if (Press_key[1][input_device::up]) {
        Player_2.dir = 2;
        --mut_dst_block_y;
      } else if (Press_key[1][input_device::right]) {
        Player_2.dir = 3;
        ++mut_dst_block_x;
      }
      const int dst_block_x = mut_dst_block_x;
      const int dst_block_y = mut_dst_block_y;

      const unsigned int dst_block_state =
          Map::check_state(dst_block_x, dst_block_y);
      const unsigned int dst_right_block_state =
          Map::check_state(dst_block_x + 1, dst_block_y);
      const unsigned int dst_left_block_state =
          Map::check_state(dst_block_x - 1, dst_block_y);
      if ((dst_block_state == 0) || (dst_block_state == 3) ||
          (dst_block_state == 4) || (dst_block_state == 5) ||
          (dst_block_state == 6) || (dst_right_block_state == 6) ||
          (dst_block_state == 7) || (dst_left_block_state == 7) ||
          (dst_block_state == 8)) {
        Player_2.next_block_x = dst_block_x;
        Player_2.next_block_y = dst_block_y;
      }

      // Circle corner
      if (Map::check_state(dst_block_x + 2, dst_block_y) == 6) {
        Player_2.next_block_x = block::count_x;
        Player_2.pos_x = block::size * Player_2.next_block_x;
      }
      if (Map::check_state(dst_block_x - 2, dst_block_y) == 7) {
        Player_2.next_block_x = -1;
        Player_2.pos_x = block::size * Player_2.next_block_x;
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

int get_player_1_life() { return Player_1.life; }

int get_player_2_life() { return Player_2.life; }

void set_player_1_life(const int life) { Player_1.life = life; }

void set_player_2_life(const int life) { Player_2.life = life; }

}  // namespace player
