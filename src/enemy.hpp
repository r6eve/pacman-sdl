#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include <memory>

class Enemy {
  typedef struct {
    int pos_x;
    int pos_y;
    int block_x;
    int block_y;
    int next_block_x;
    int next_block_y;
    unsigned char dir;           // max value is 4
    unsigned char anime_count;   // 0 or 1
    unsigned char anime_weight;  // max value is 8
  } Enemy_data;

  std::unique_ptr<Enemy_data[]> enemies_;

  void update();

 public:
  Enemy() : enemies_(new Enemy_data[enemy_character::count]) {}

  void init();

  void draw() const;

  // TODO: take parameter as enemy_charater
  void move_normal_enemy(unsigned int enemy_type);

  // TODO: take parameter as enemy_charater
  void move_lose_enemy(unsigned int enemy_type);

  /**
   * Return true if the player whose state is normal hits enemy, and false
   * otherwise.
   */
  bool check_hit_enemy() const;

  ~Enemy() {}
};

#endif
