#ifndef PLAYER_H
#define PLAYER_H

namespace player {

void init();
void draw();
void move();

// TODO: use class
void add_player_1_pos(int x, int y);
void add_player_2_pos(int x, int y);
int get_player_1_pos_x();
int get_player_2_pos_x();
int get_player_1_pos_y();
int get_player_2_pos_y();
int get_player_1_block_x();
int get_player_2_block_x();
int get_player_1_block_y();
int get_player_2_block_y();
int get_player_1_life();
int get_player_2_life();
void set_player_1_life(const int life);
void set_player_2_life(const int life);

}  // namespace player

#endif
