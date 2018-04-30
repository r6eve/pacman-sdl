#include "main_chara.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input.hpp"
#include "map.hpp"

static Chara Main_chara;
static Chara Rival_chara;

void init_main_chara() {
  Main_chara.pos_x = BLOCK_SIZE * 9;
  Main_chara.pos_y = BLOCK_SIZE * 18;
  Main_chara.nextblock_x = Main_chara.block_x = 9;
  Main_chara.nextblock_y = Main_chara.block_y = 18;
  Main_chara.dir = 1;
  Main_chara.anime_count = 0;
  Main_chara.anime_weight = 0;
  if (Game_mode == GAME_MODE_BATTLE) {
    Rival_chara.pos_x = BLOCK_SIZE * 14;
    Rival_chara.pos_y = BLOCK_SIZE * 18;
    Rival_chara.nextblock_x = Main_chara.block_x = 14;
    Rival_chara.nextblock_y = Main_chara.block_y = 18;
    Rival_chara.dir = 3;
    Rival_chara.anime_count = 0;
    Rival_chara.anime_weight = 0;
  }
}

void update_main_chara(int index) {
  if (index == 0) {
    ++Main_chara.anime_weight;
    if (Main_chara.anime_weight > 4) {
      Main_chara.anime_count = 1 - Main_chara.anime_count;
      Main_chara.anime_weight = 0;
    }
  } else if (index == 1) {
    ++Rival_chara.anime_weight;
    if (Rival_chara.anime_weight > 4) {
      Rival_chara.anime_count = 1 - Rival_chara.anime_count;
      Rival_chara.anime_weight = 0;
    }
  } // TODO: else
}

void draw_main_chara() {
  SDL_Surface *p_surface = get_img("pacman");
  SDL_Rect src;
  src.x = BLOCK_SIZE * Main_chara.dir;
  src.y = BLOCK_SIZE * Main_chara.anime_count;
  src.w = BLOCK_SIZE;
  src.h = BLOCK_SIZE;
  SDL_Rect dst;
  dst.x = Main_chara.pos_x;
  dst.y = Main_chara.pos_y;
  SDL_BlitSurface(p_surface, &src, Screen, &dst);
  if (Game_mode == GAME_MODE_BATTLE) {
    SDL_Surface *p_rival = get_img("rival");
    SDL_Rect src_rival;
    src_rival.x = BLOCK_SIZE * Rival_chara.dir;
    src_rival.y = BLOCK_SIZE * Rival_chara.anime_count;
    src_rival.w = BLOCK_SIZE;
    src_rival.h = BLOCK_SIZE;
    SDL_Rect dst_rival;
    dst_rival.x = Rival_chara.pos_x;
    dst_rival.y = Rival_chara.pos_y;
    SDL_BlitSurface(p_rival, &src_rival, Screen, &dst_rival);
  }
}

// TODO: reduce magic numbers
void mv_main_chara() {
  int is_mving;
  int dst_pos_x = Main_chara.nextblock_x * BLOCK_SIZE;
  int dst_pos_y = Main_chara.nextblock_y * BLOCK_SIZE;

  if ((Main_chara.pos_x == dst_pos_x) && (Main_chara.pos_y == dst_pos_y)) {
    is_mving = 0;
  } else {
    is_mving = 1;
  }

  if (is_mving) {
    update_main_chara(0);
    int mv_value = 2;
    if (dst_pos_x > Main_chara.pos_x) {
      Main_chara.pos_x += mv_value;
    } else if (dst_pos_x < Main_chara.pos_x) {
      Main_chara.pos_x -= mv_value;
    }
    if (dst_pos_y > Main_chara.pos_y) {
      Main_chara.pos_y += mv_value;
    } else if (dst_pos_y < Main_chara.pos_y) {
      Main_chara.pos_y -= mv_value;
    }
  } else {
    int dst_block_x = Main_chara.nextblock_x;
    int dst_block_y = Main_chara.nextblock_y;
    Main_chara.block_x = dst_block_x;
    Main_chara.block_y = dst_block_y;

    if (Press_key[0][PRESS_KEY_DOWN]) {
      Main_chara.dir = 0;
      ++dst_block_y;
    } else if (Press_key[0][PRESS_KEY_LEFT]) {
      Main_chara.dir = 1;
      --dst_block_x;
    } else if (Press_key[0][PRESS_KEY_UP]) {
      Main_chara.dir = 2;
      --dst_block_y;
    } else if (Press_key[0][PRESS_KEY_RIGHT]) {
      Main_chara.dir = 3;
      ++dst_block_x;
    }

    if ((check_map_state(dst_block_x, dst_block_y) == 0) ||
        (check_map_state(dst_block_x, dst_block_y) == 3) ||
        (check_map_state(dst_block_x, dst_block_y) == 4) ||
        (check_map_state(dst_block_x, dst_block_y) == 5) ||
        (check_map_state(dst_block_x, dst_block_y) == 6) ||
        (check_map_state(dst_block_x + 1, dst_block_y) == 6) ||
        (check_map_state(dst_block_x, dst_block_y) == 7) ||
        (check_map_state(dst_block_x - 1, dst_block_y) == 7) ||
        (check_map_state(dst_block_x, dst_block_y) == 8)) {
      Main_chara.nextblock_x = dst_block_x;
      Main_chara.nextblock_y = dst_block_y;
    }

    if (check_map_state(dst_block_x + 2, dst_block_y) == 6) {
      Main_chara.nextblock_x = NUM_BLOCK_X;
      Main_chara.pos_x = BLOCK_SIZE * Main_chara.nextblock_x;
    }

    if (check_map_state(dst_block_x - 2, dst_block_y) == 7) {
      Main_chara.nextblock_x = -1;
      Main_chara.pos_x = BLOCK_SIZE * Main_chara.nextblock_x;
    }
  }

  if (Game_mode == GAME_MODE_BATTLE) {
    int is_mving;
    int dst_pos_x = Rival_chara.nextblock_x * BLOCK_SIZE;
    int dst_pos_y = Rival_chara.nextblock_y * BLOCK_SIZE;

    if ((Rival_chara.pos_x == dst_pos_x) && (Rival_chara.pos_y == dst_pos_y)) {
      is_mving = 0;
    } else {
      is_mving = 1;
    }

    if (is_mving) {
      update_main_chara(1);
      int mv_value = 2;
      if (dst_pos_x > Rival_chara.pos_x) {
        Rival_chara.pos_x += mv_value;
      } else if (dst_pos_x < Rival_chara.pos_x) {
        Rival_chara.pos_x -= mv_value;
      }

      if (dst_pos_y > Rival_chara.pos_y) {
        Rival_chara.pos_y += mv_value;
      } else if (dst_pos_y < Rival_chara.pos_y) {
        Rival_chara.pos_y -= mv_value;
      }
    } else {
      int dst_block_x = Rival_chara.nextblock_x;
      int dst_block_y = Rival_chara.nextblock_y;
      Rival_chara.block_x = dst_block_x;
      Rival_chara.block_y = dst_block_y;

      if (Press_key[1][PRESS_KEY_DOWN]) {
        Rival_chara.dir = 0;
        ++dst_block_y;
      } else if (Press_key[1][PRESS_KEY_LEFT]) {
        Rival_chara.dir = 1;
        --dst_block_x;
      } else if (Press_key[1][PRESS_KEY_UP]) {
        Rival_chara.dir = 2;
        --dst_block_y;
      } else if (Press_key[1][PRESS_KEY_RIGHT]) {
        Rival_chara.dir = 3;
        ++dst_block_x;
      }

      if ((check_map_state(dst_block_x, dst_block_y) == 0) ||
          (check_map_state(dst_block_x, dst_block_y) == 3) ||
          (check_map_state(dst_block_x, dst_block_y) == 4) ||
          (check_map_state(dst_block_x, dst_block_y) == 5) ||
          (check_map_state(dst_block_x, dst_block_y) == 6) ||
          (check_map_state(dst_block_x + 1, dst_block_y) == 6) ||
          (check_map_state(dst_block_x, dst_block_y) == 7) ||
          (check_map_state(dst_block_x - 1, dst_block_y) == 7) ||
          (check_map_state(dst_block_x, dst_block_y) == 8)) {
        Rival_chara.nextblock_x = dst_block_x;
        Rival_chara.nextblock_y = dst_block_y;
      }

      if (check_map_state(dst_block_x + 2, dst_block_y) == 6) {
        Rival_chara.nextblock_x = NUM_BLOCK_X;
        Rival_chara.pos_x = BLOCK_SIZE * Rival_chara.nextblock_x;
      }

      if (check_map_state(dst_block_x - 2, dst_block_y) == 7) {
        Rival_chara.nextblock_x = -1;
        Rival_chara.pos_x = BLOCK_SIZE * Rival_chara.nextblock_x;
      }
    }
  }
}

void add_main_chara_pos(int x, int y) {
  Main_chara.pos_x += x;
  Main_chara.pos_y += y;
}

void add_rival_chara_pos(int x, int y) {
  Rival_chara.pos_x += x;
  Rival_chara.pos_y += y;
}

int get_main_chara_pos_x() { return Main_chara.pos_x; }

int get_rival_chara_pos_x() { return Rival_chara.pos_x; }

int get_main_chara_pos_y() { return Main_chara.pos_y; }

int get_rival_chara_pos_y() { return Rival_chara.pos_y; }

int get_main_chara_block_x() { return Main_chara.block_x; }

int get_rival_chara_block_x() { return Rival_chara.block_x; }

int get_main_chara_block_y() { return Main_chara.block_y; }

int get_rival_chara_block_y() { return Rival_chara.block_y; }
