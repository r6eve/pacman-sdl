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
      food_[y][x] = -1;
      if (map.check_state(Point{x, y}) == 0) {  // field where player can move
        food_[y][x] = 1;
      } else if (map.check_state(Point{x, y}) == 4) {  // counter food
        food_[y][x] = 0;
      }
    }
  }
}

void Food::draw(SDL_Surface *screen, const ImageManager &image_manager) noexcept {
  // update
  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      if (food_[y][x] == 2) {
        food_[y][x] = -1;
      }
    }
  }

  SDL_Rect src = {0, 0, block::size, block::size};
  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      SDL_Rect dst = {static_cast<Sint16>(block::size * x),
                      static_cast<Sint16>(block::size * y), 0, 0};
      if (food_[y][x] == 1) {  // food
        SDL_Surface *p_surface = image_manager.get(image::food);
        SDL_BlitSurface(p_surface, &src, screen, &dst);
      } else if (food_[y][x] == 0) {  // counter food
        SDL_Surface *p_surface = image_manager.get(image::food_counter);
        SDL_BlitSurface(p_surface, &src, screen, &dst);
      }
    }
  }
}

bool Food::check_state(const game_mode mode, const MixerManager &mixer_manager,
                       Player &p1, Player &p2) noexcept {
  const Point block = p1.get_block();
  if (food_[block.y][block.x] == 1) {
    Mix_PlayChannel(-1, mixer_manager.get_se("chomp"), 0);
    ++food_[block.y][block.x];
    p1.set_score(p1.get_score() + 10);
  }
  if (food_[block.y][block.x] == 0) {
    p1.set_power_mode(400);
    Mix_PlayMusic(mixer_manager.get_music("siren"), -1);
    food_[block.y][block.x] += 2;
  }
  if ((p1.get_power_mode() == 0) && (p2.get_power_mode() == 0)) {
    while (!Mix_FadeOutMusic(800) && Mix_PlayingMusic()) {
      ;
    }
  }

  if (mode == game_mode::battle) {
    const Point block = p2.get_block();
    if (food_[block.y][block.x] == 1) {
      Mix_PlayChannel(-1, mixer_manager.get_se("chomp"), 0);
      ++food_[block.y][block.x];
      p2.set_score(p2.get_score() + 10);
    }
    if (food_[block.y][block.x] == 0) {
      p2.set_power_mode(400);
      Mix_PlayMusic(mixer_manager.get_music("siren"), -1);
      food_[block.y][block.x] += 2;
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
      if ((food_[y][x] == 0) || (food_[y][x] == 1)) {
        ++rest_food;
      }
    }
  }

  return (rest_food == 0);
}
