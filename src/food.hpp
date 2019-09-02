//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <SDL2/SDL_mixer.h>
#include "def_global.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"

class Food {
  enum class food_state {
    nothing,
    food,
    counter_food,
  };

  food_state food_[block::count_y][block::count_x];
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;

 public:
  Food(const ImageManager *image_manager,
       const MixerManager *mixer_manager) noexcept
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {}

  inline void init(const Map &map) noexcept {
    for (int y = 0; y < block::count_y; ++y) {
      for (int x = 0; x < block::count_x; ++x) {
        switch (map.check_state(Point{x, y})) {
          case map_state::food:
            food_[y][x] = food_state::food;
            break;
          case map_state::counter_food:
            food_[y][x] = food_state::counter_food;
            break;
          default:
            food_[y][x] = food_state::nothing;
            break;
        }
      }
    }
  }

  inline void draw() noexcept {
    const SDL_Rect src = {0, 0, block::size, block::size};
    SDL_Texture *food_texture = image_manager_->get(image::food);
    SDL_Texture *food_counter_texture = image_manager_->get(image::food_counter);
    for (unsigned int y = 0; y < block::count_y; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        SDL_Rect dst;
        dst.x = static_cast<Sint16>(block::size * x);
        dst.y = static_cast<Sint16>(block::size * y);
        switch (food_[y][x]) {
          case food_state::food: {
            SDL_QueryTexture(food_texture, nullptr, nullptr, &dst.w, &dst.h);
            image_manager_->render_copy(*food_texture, src, dst);
            break;
          }
          case food_state::counter_food: {
            SDL_QueryTexture(food_counter_texture, nullptr, nullptr, &dst.w,
                             &dst.h);
            image_manager_->render_copy(*food_counter_texture, src, dst);
            break;
          }
          default:
            // do nothing
            break;
        }
      }
    }
    SDL_DestroyTexture(food_texture);
    SDL_DestroyTexture(food_counter_texture);
  }

  /**
   * Return true if all the foods are eaten, and false otherwise.
   */
  inline bool check_state(const game_mode mode, Player &p1,
                          Player &p2) noexcept {
    const Point block = p1.get_block();
    switch (food_[block.y][block.x]) {
      case food_state::food: {
        Mix_PlayChannel(se_type::chomp, mixer_manager_->get_se(se_type::chomp),
                        0);
        food_[block.y][block.x] = food_state::nothing;
        p1.set_score(p1.get_score() + 10);
        break;
      }
      case food_state::counter_food: {
        p1.set_power_mode(400);
        food_[block.y][block.x] = food_state::nothing;
        break;
      }
      default:
        // do nothing
        break;
    }

    if (mode == game_mode::battle) {
      const Point block = p2.get_block();
      switch (food_[block.y][block.x]) {
        case food_state::food: {
          Mix_PlayChannel(se_type::chomp,
                          mixer_manager_->get_se(se_type::chomp), 0);
          food_[block.y][block.x] = food_state::nothing;
          p2.set_score(p2.get_score() + 10);
          break;
        }
        case food_state::counter_food: {
          p2.set_power_mode(400);
          food_[block.y][block.x] = food_state::nothing;
          break;
        }
        default:
          // do nothing
          break;
      }
    }

    if ((p1.get_power_mode() != 0 && p1.get_power_mode() % 80 == 0)
        || (p2.get_power_mode() != 0 && p2.get_power_mode() % 80 == 0)) {
      Mix_PlayChannel(se_type::siren, mixer_manager_->get_se(se_type::siren),
                      0);
    }

    int rest_food = 0;
    for (unsigned int y = 0; y < block::count_y; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        if (food_[y][x] == food_state::food
            || food_[y][x] == food_state::counter_food) {
          ++rest_food;
        }
      }
    }

    return rest_food == 0;
  }

  ~Food() noexcept {}
};
