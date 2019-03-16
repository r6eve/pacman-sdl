//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>
#include <vector>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
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
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;

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
  Enemy(const ImageManager *image_manager,
        const MixerManager *mixer_manager) noexcept
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {
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

  inline void draw() const noexcept {
    SDL_Texture *enemies_texture[enemy_character::count];
    enemies_texture[enemy_character::akabei] = image_manager_->get(image::akabei);
    enemies_texture[enemy_character::pinky] = image_manager_->get(image::pinky);
    enemies_texture[enemy_character::aosuke] = image_manager_->get(image::aosuke);
    enemies_texture[enemy_character::guzuta] = image_manager_->get(image::guzuta);
    SDL_Texture *mon_run_texture = image_manager_->get(image::mon_run);
    for (const auto &enemy : enemies_) {
      const SDL_Rect dst = {static_cast<Sint16>(enemy.pos.x),
                            static_cast<Sint16>(enemy.pos.y),
                            block::size,
                            block::size};
      switch (enemy.state) {
        case enemy_state::normal: {
          const SDL_Rect src = {static_cast<Sint16>(block::size * enemy.dir),
                                static_cast<Sint16>(block::size * enemy.anime_count),
                                block::size,
                                block::size};
          image_manager_->render_copy(*enemies_texture[enemy.type], src, dst);
          break;
        }
        case enemy_state::lose: {
          const SDL_Rect src = {0,
                                static_cast<Sint16>(block::size * enemy.anime_count),
                                block::size,
                                block::size};
          image_manager_->render_copy(*mon_run_texture, src, dst);
          break;
        }
      }
    }

    for (auto &t : enemies_texture) {
      SDL_DestroyTexture(t);
    }
    SDL_DestroyTexture(mon_run_texture);
  }

  inline void move(const bool debug_lose_enemy, const Map &map,
                   const Player &p1, const Player &p2) noexcept {
    for (auto &enemy : enemies_) {
      if (debug_lose_enemy || enemy.state == enemy_state::lose) {
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

  ~Enemy() noexcept {}
};
