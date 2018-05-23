#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include <vector>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
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

  struct Enemy_data {
    const unsigned char type;
    Point pos;
    Point block;
    Point next_block;
    unsigned char dir;           // max value is 4
    unsigned char anime_count;   // 0 or 1
    unsigned char anime_weight;  // max value is 8
    enemy_state state;

    Enemy_data(const unsigned char enemy_type) : type(enemy_type) {}
  };

  std::vector<Enemy_data> enemies_;

  inline void update() noexcept {
    for (auto &enemy : enemies_) {
      ++enemy.anime_weight;
      if (enemy.anime_weight >= 8) {
        enemy.anime_weight = 0;
        ++enemy.anime_count;
        enemy.anime_count %= 2;
      }
    }
  }

  void move_normal_enemy(Enemy_data &enemy, const Map &map, const Player &p1,
                         const Player &p2) noexcept;

  void move_lose_enemy(Enemy_data &enemy, const Map &map, const Player &p1,
                       const Player &p2) noexcept;

 public:
  Enemy() {
    enemies_.reserve(enemy_character::count);
    for (unsigned char i = 0; i < enemy_character::count; ++i) {
      enemies_.push_back(Enemy_data(i));
    }
  }

  inline void init() noexcept {
    const Point start_block[enemy_character::count] = {
        {11, 12}, {12, 12}, {11, 11}, {12, 11}};
    for (auto &enemy : enemies_) {
      enemy.pos = {block::size * start_block[enemy.type].x,
                   block::size * start_block[enemy.type].y};
      enemy.block = start_block[enemy.type];
      enemy.next_block = start_block[enemy.type];
      enemy.dir = 2;
      enemy.anime_count = 0;
      enemy.anime_weight = 0;
      enemy.state = enemy_state::normal;
    }
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    SDL_Surface *enemies_surface[enemy_character::count];
    enemies_surface[enemy_character::akabei] = image_manager.get(image::akabei);
    enemies_surface[enemy_character::pinky] = image_manager.get(image::pinky);
    enemies_surface[enemy_character::aosuke] = image_manager.get(image::aosuke);
    enemies_surface[enemy_character::guzuta] = image_manager.get(image::guzuta);
    SDL_Surface *mon_run_surface = image_manager.get(image::mon_run);
    for (const auto &enemy : enemies_) {
      SDL_Rect dst = {static_cast<Sint16>(enemy.pos.x),
                      static_cast<Sint16>(enemy.pos.y), 0, 0};
      switch (enemy.state) {
        case enemy_state::normal: {
          SDL_Rect src = {static_cast<Sint16>(block::size * enemy.dir),
                          static_cast<Sint16>(block::size * enemy.anime_count),
                          block::size, block::size};
          SDL_BlitSurface(enemies_surface[enemy.type], &src, screen, &dst);
          break;
        }
        case enemy_state::lose: {
          SDL_Rect src = {0,
                          static_cast<Sint16>(block::size * enemy.anime_count),
                          block::size, block::size};
          SDL_BlitSurface(mon_run_surface, &src, screen, &dst);
          break;
        }
        default:
          // NOTREACHED
          break;
      }
    }
  }

  inline void move(const bool debug_lose_enemy, const Map &map,
                   const Player &p1, const Player &p2) noexcept {
    for (auto &enemy : enemies_) {
      if (debug_lose_enemy || (enemy.state == enemy_state::lose)) {
        move_lose_enemy(enemy, map, p1, p2);
      } else {
        move_normal_enemy(enemy, map, p1, p2);
      }
    }
  }

  /**
   * Return true if the player whose state is normal hits enemy, and false
   * otherwise.
   */
  bool check_hit_enemy(const game_mode mode, Player &p1, Player &p2) noexcept;

  ~Enemy() {}
};

#endif
