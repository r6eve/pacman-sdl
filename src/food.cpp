#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "enemy.hpp"
#include "food.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"

void Food::init(const Map &map) noexcept {
  for (int y = 0; y < block::count_y; ++y) {
    for (int x = 0; x < block::count_x; ++x) {
      const map_state state = map.check_state(Point{x, y});
      if (state == map_state::food) {
        food_[y][x] = food_state::food;
      } else if (state == map_state::counter_food) {
        food_[y][x] = food_state::counter_food;
      } else {
        food_[y][x] = food_state::nothing;
      }
    }
  }
}

void Food::draw(SDL_Surface *screen,
                const ImageManager &image_manager) noexcept {
  SDL_Rect src = {0, 0, block::size, block::size};
  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                      static_cast<Sint16>(block::size * y), 0, 0};
      switch (food_[y][x]) {
        case food_state::food: {
          SDL_Surface *p_surface = image_manager.get(image::food);
          SDL_BlitSurface(p_surface, &src, screen, &dst);
          break;
        }
        case food_state::counter_food: {
          SDL_Surface *p_surface = image_manager.get(image::food_counter);
          SDL_BlitSurface(p_surface, &src, screen, &dst);
          break;
        }
        default:
          // do nothing
          break;
      }
    }
  }
}

bool Food::check_state(const game_mode mode, const MixerManager &mixer_manager,
                       Player &p1, Player &p2) noexcept {
  const Point block = p1.get_block();
  switch (food_[block.y][block.x]) {
    case food_state::food: {
      Mix_PlayChannel(-1, mixer_manager.get_se(), 0);
      food_[block.y][block.x] = food_state::nothing;
      p1.set_score(p1.get_score() + 10);
      break;
    }
    case food_state::counter_food: {
      p1.set_power_mode(400);
      Mix_PlayMusic(mixer_manager.get_music(music_type::siren), -1);
      food_[block.y][block.x] = food_state::nothing;
      break;
    }
    default:
      // do nothing
      break;
  }
  if ((p1.get_power_mode() == 0) && (p2.get_power_mode() == 0)) {
    while (!Mix_FadeOutMusic(800) && Mix_PlayingMusic()) {
      ;
    }
  }

  if (mode == game_mode::battle) {
    const Point block = p2.get_block();
    switch (food_[block.y][block.x]) {
      case food_state::food: {
        Mix_PlayChannel(-1, mixer_manager.get_se(), 0);
        food_[block.y][block.x] = food_state::nothing;
        p2.set_score(p2.get_score() + 10);
        break;
      }
      case food_state::counter_food: {
        p2.set_power_mode(400);
        Mix_PlayMusic(mixer_manager.get_music(music_type::siren), -1);
        food_[block.y][block.x] = food_state::nothing;
        break;
      }
      default:
        // do nothing
        break;
    }
    if ((p1.get_power_mode() == 0) && (p2.get_power_mode() == 0)) {
      while (!Mix_FadeOutMusic(800) && Mix_PlayingMusic()) {
        ;
      }
    }
  }

  int rest_food = 0;
  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      if ((food_[y][x] == food_state::food) ||
          (food_[y][x] == food_state::counter_food)) {
        ++rest_food;
      }
    }
  }

  return (rest_food == 0);
}
