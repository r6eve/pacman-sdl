#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include "def_global.hpp"
#include "player.hpp"

namespace enemy_character {

enum {
  akabei = 0,
  pinky,
  aosuke,
  guzuta,
  count,
};

}  // namespace enemy_character

class Enemy {
  enum class enemy_state {
    normal,
    lose,
  };

  typedef struct {
    Point pos;
    Point block;
    Point next_block;
    unsigned char dir;           // max value is 4
    unsigned char anime_count;   // 0 or 1
    unsigned char anime_weight;  // max value is 8
    enemy_state state;
  } Enemy_data;

  std::unique_ptr<Enemy_data[]> enemies_;

  void update() noexcept;

  void move_normal_enemy(const unsigned int enemy_type, const Player &player1,
                         const Player &player2) noexcept;

  void move_lose_enemy(const unsigned int enemy_type, const Player &player1,
                       const Player &player2) noexcept;

 public:
  Enemy() : enemies_(std::make_unique<Enemy_data[]>(enemy_character::count)) {}

  void init() noexcept;

  void draw() const noexcept;

  void move(const bool debug_lose_enemy, const Player &player1,
            const Player &player2) noexcept;

  /**
   * Return true if the player whose state is normal hits enemy, and false
   * otherwise.
   */
  bool check_hit_enemy(Player &player1, Player &player2) const noexcept;

  ~Enemy() {}
};

#endif
