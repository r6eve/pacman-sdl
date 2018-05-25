#ifndef FOOD_H
#define FOOD_H

#include "def_global.hpp"
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

 public:
  Food() {}

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

  inline void draw(SDL_Renderer *renderer,
                   const ImageManager &image_manager) noexcept {
    SDL_Rect src = {0, 0, block::size, block::size};
    for (unsigned int y = 0; y < block::count_y; ++y) {
      for (unsigned int x = 0; x < block::count_x; ++x) {
        SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                        static_cast<Sint16>(block::size * y), 0, 0};
        switch (food_[y][x]) {
          case food_state::food: {
            SDL_Texture *p_texture = image_manager.get(renderer, image::food);
            SDL_RenderCopy(renderer, p_texture, &src, &dst);
            break;
          }
          case food_state::counter_food: {
            SDL_Texture *p_texture = image_manager.get(renderer, image::food_counter);
            SDL_RenderCopy(renderer, p_texture, &src, &dst);
            break;
          }
          default:
            // do nothing
            break;
        }
      }
    }
  }

  /**
   * Return true if all of the food are eaten, and false otherwise.
   */
  bool check_state(const game_mode mode, const MixerManager &mixer_manager,
                   Player &p1, Player &p2) noexcept;

  ~Food() {}
};

#endif
