#ifndef PLAYER_H
#define PLAYER_H

namespace player {

// TODO: private class
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

void init_player();
void draw_player();
void move_player();
void add_player_1_pos(int x, int y);
void add_player_2_pos(int x, int y);

// TODO: use class
int get_player_1_pos_x();
int get_player_2_pos_x();
int get_player_1_pos_y();
int get_player_2_pos_y();
int get_player_1_block_x();
int get_player_2_block_x();
int get_player_1_block_y();
int get_player_2_block_y();

}  // namespace player

#endif
