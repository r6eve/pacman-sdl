#include "food.hpp"
#include <SDL2/SDL_mixer.h>
#include "def_global.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"

bool Food::check_state(const game_mode mode, const MixerManager &mixer_manager,
                       Player &p1, Player &p2) noexcept {
  const Point block = p1.get_block();
  switch (food_[block.y][block.x]) {
    case food_state::food: {
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::chomp), 0);
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
        Mix_PlayChannel(-1, mixer_manager.get_se(se_type::chomp), 0);
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

  if (((p1.get_power_mode() != 0) && (p1.get_power_mode() % 80 == 0)) ||
      ((p2.get_power_mode() != 0) && (p2.get_power_mode() % 80 == 0))) {
    Mix_PlayChannel(0, mixer_manager.get_se(se_type::siren), 0);
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
