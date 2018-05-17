#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
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

  typedef struct {
    Point pos;
    Point block;
    Point next_block;
    unsigned char dir;           // max value is 4
    unsigned char anime_count;   // 0 or 1
    unsigned char anime_weight;  // max value is 8
    enemy_state state;
  } Enemy_data;

  // TODO: Why create those in heap?
  std::unique_ptr<Enemy_data[]> enemies_;

  inline void update() noexcept {
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      ++enemies_[i].anime_weight;
      if (enemies_[i].anime_weight >= 8) {
        enemies_[i].anime_weight = 0;
        ++enemies_[i].anime_count;
        enemies_[i].anime_count %= 2;
      }
    }
  }

  void move_normal_enemy(const unsigned int enemy_type, const Map &map,
                         const Player &p1, const Player &p2) noexcept;

  void move_lose_enemy(const unsigned int enemy_type, const Map &map,
                       const Player &p1, const Player &p2) noexcept;

 public:
  Enemy() : enemies_(std::make_unique<Enemy_data[]>(enemy_character::count)) {}

  inline void init() noexcept {
    const Point start_block[enemy_character::count] = {
        {11, 12}, {12, 12}, {11, 11}, {12, 11}};
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      enemies_[i].pos = {block::size * start_block[i].x,
                         block::size * start_block[i].y};
      enemies_[i].block = start_block[i];
      enemies_[i].next_block = start_block[i];
      enemies_[i].dir = 2;
      enemies_[i].anime_count = 0;
      enemies_[i].anime_weight = 0;
      enemies_[i].state = enemy_state::normal;
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
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      SDL_Rect dst = {static_cast<Sint16>(enemies_[i].pos.x),
                      static_cast<Sint16>(enemies_[i].pos.y), 0, 0};
      switch (enemies_[i].state) {
        case enemy_state::normal: {
          SDL_Rect src = {
              static_cast<Sint16>(block::size * enemies_[i].dir),
              static_cast<Sint16>(block::size * enemies_[i].anime_count),
              block::size, block::size};
          SDL_BlitSurface(enemies_surface[i], &src, screen, &dst);
          break;
        }
        case enemy_state::lose: {
          SDL_Rect src = {
              0, static_cast<Sint16>(block::size * enemies_[i].anime_count),
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

  inline void move(const bool debug_lose_enemy, const Map &map, const Player &p1,
            const Player &p2) noexcept {
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      if (debug_lose_enemy || (enemies_[i].state == enemy_state::lose)) {
        move_lose_enemy(i, map, p1, p2);
      } else {
        move_normal_enemy(i, map, p1, p2);
      }
    }
  }

  /**
   * Return true if the player whose state is normal hits enemy, and false
   * otherwise.
   */
  bool check_hit_enemy(const game_mode mode, Player &p1, Player &p2) const
      noexcept;

  ~Enemy() {}
};

#endif
