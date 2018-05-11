#ifndef PLAYER_H
#define PLAYER_H

class Player {
  int pos_x_;
  int pos_y_;
  int block_x_;
  int block_y_;
  int next_block_x_;
  int next_block_y_;
  unsigned char dir_;           // 0: down, 1: left, 2: up, 3: right
  unsigned char anime_count_;   // 0 or 1
  unsigned char anime_weight_;  // max value is 4
  int life_;

 public:
  Player() {}
  void init_pos(const unsigned int player_type) noexcept;
  void draw(const unsigned int player_type) const noexcept;
  void move(const unsigned int player_type) noexcept;
  void add_pos(int x, int y) noexcept;
  int get_pos_x() const noexcept;
  int get_pos_y() const noexcept;
  int get_block_x() const noexcept;
  int get_block_y() const noexcept;
  int get_life() const noexcept;
  void set_life(const int life) noexcept;
  ~Player() {}
};

#endif
