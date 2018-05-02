#ifndef MAIN_CHARA_H
#define MAIN_CHARA_H

namespace main_chara {

// TODO: private class
// TODO: add a member of life descriving Main_chara_life/Rival_chara_life
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

void init_main_chara();
void draw_main_chara();
void move_main_chara();
void add_main_chara_pos(int x, int y);
void add_rival_chara_pos(int x, int y);

// TODO: use class
int get_main_chara_pos_x();
int get_rival_chara_pos_x();
int get_main_chara_pos_y();
int get_rival_chara_pos_y();
int get_main_chara_block_x();
int get_rival_chara_block_x();
int get_main_chara_block_y();
int get_rival_chara_block_y();

}  // namespace main_chara

#endif
