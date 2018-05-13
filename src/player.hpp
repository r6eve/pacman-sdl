#ifndef PLAYER_H
#define PLAYER_H

#include "def_global.hpp"

class Player {
  const unsigned int player_type_;
  Point pos_;
  Point block_;
  Point next_block_;
  unsigned char dir_;           // 0: down, 1: left, 2: up, 3: right
  unsigned char anime_count_;   // 0 or 1
  unsigned char anime_weight_;  // max value is 4
  int life_;
  unsigned int score_;
  bool damaged_p_;

 public:
  Player(const unsigned int player_type) : player_type_(player_type) {}
  void init_pos() noexcept;
  void draw() const noexcept;
  void move() noexcept;
  Point get_pos() const noexcept;
  void set_pos(const Point &p) noexcept;
  void set_pos(const Point &&p) noexcept;
  Point get_block() const noexcept;
  int get_life() const noexcept;
  void set_life(const int life) noexcept;
  unsigned int get_score() const noexcept;
  void set_score(const unsigned int score) noexcept;
  bool get_damaged() const noexcept;
  void set_damaged(const bool damaged_p) noexcept;
  ~Player() {}
};

#endif
