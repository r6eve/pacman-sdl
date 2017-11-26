#ifndef ENEMY_H
#define ENEMY_H

typedef struct {
  int pos_x;
  int pos_y;
  int block_x;
  int block_y;
  int nextblock_x;
  int nextblock_y;
  int dir; // 0:down, 1:left, 2:up, 3:right
  int anime_count; // 0 or 1
  int anime_weight;
} Enemy_data;

void init_enemy();
void update_enemy();
void draw_enemy();
void mv_enemy(int enemy_type);
void mv_normal_enemy(int enemy_type);
void mv_lose_enemy(int enemy_type);
void check_hit_enemy();

#endif /* ENEMY_H */
