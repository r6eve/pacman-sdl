#ifndef PLAYER_H
#define PLAYER_H

namespace player {

void init();
void draw();
void move();
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
