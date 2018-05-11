#include <SDL/SDL_mixer.h>

#include "def_global.hpp"
#include "enemy.hpp"
#include "food.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"

void Food::init() noexcept {
  for (unsigned int y = 0; y < block::count_y; ++y) {
    for (unsigned int x = 0; x < block::count_x; ++x) {
      food_[y][x] = -1;
      if (Map::check_state(x, y) == 0) {  // field where player can move
        food_[y][x] = 1;
      } else if (Map::check_state(x, y) == 4) {  // counter food
        food_[y][x] = 0;
      }
    }
  }
}

void Food::draw() noexcept {
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
        SDL_Surface *p_surface = Image_manager::get("food");
        SDL_BlitSurface(p_surface, &src, Screen, &dst);
      } else if (food_[y][x] == 0) {  // counter food
        SDL_Surface *p_surface = Image_manager::get("food_counter");
        SDL_BlitSurface(p_surface, &src, Screen, &dst);
      }
    }
  }
}

bool Food::check_state(Player &player1, Player &player2) noexcept {
  const Point block = player1.get_block();
  if (food_[block.y][block.x] == 1) {
    Mix_PlayChannel(-1, Mixer_manager::get_se("chomp"), 0);
    ++food_[block.y][block.x];
    player1.set_score(player1.get_score() + 10);
  }
  if (food_[block.y][block.x] == 0) {
    Power_chara_mode[0] = 400;
    Mix_PlayMusic(Mixer_manager::get_music("siren"), -1);
    food_[block.y][block.x] += 2;
  }
  if ((Power_chara_mode[0] == 0) && (Power_chara_mode[1] == 0)) {
    while (!Mix_FadeOutMusic(800) && Mix_PlayingMusic()) {
      ;
    }
  }

  if (Game_mode == game_mode::battle) {
    const Point block = player2.get_block();
    if (food_[block.y][block.x] == 1) {
      Mix_PlayChannel(-1, Mixer_manager::get_se("chomp"), 0);
      ++food_[block.y][block.x];
      player2.set_score(player2.get_score() + 10);
    }
    if (food_[block.y][block.x] == 0) {
      Power_chara_mode[1] = 400;
      Mix_PlayMusic(Mixer_manager::get_music("siren"), -1);
      food_[block.y][block.x] += 2;
    }
    if ((Power_chara_mode[0] == 0) && (Power_chara_mode[1] == 0)) {
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
