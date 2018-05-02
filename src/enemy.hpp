#ifndef ENEMY_H
#define ENEMY_H

namespace enemy {

typedef struct {
  int pos_x;
  int pos_y;
  int block_x;
  int block_y;
  int nextblock_x;
  int nextblock_y;
  int dir;          // TODO: enum class 0:down, 1:left, 2:up, 3:right
  int anime_count;  // 0 or 1
  int anime_weight;
} Enemy_data;

void init_enemy();

void draw_enemy();

void move_normal_enemy(int enemy_type);

void move_lose_enemy(int enemy_type);

/**
 * Return true if the player whose state is normal hits enemy, and false
 * otherwise.
 */
bool check_hit_enemy();

}  // namespace enemy

#endif
