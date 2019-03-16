//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#include "pacman.hpp"
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "def_global.hpp"
#include "enemy.hpp"
#include "font_manager.hpp"
#include "food.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"
#include "wipe.hpp"

void Pacman::game_title() noexcept {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);

  const Point title_pos = Point{160, 160};
  const Point p1_mode_pos = Point{270, 300};
  const Point vs_mode_pos = Point{270, 350};
  const char *title_str = "P  a  c  -  M  a  n";
  const char *p1_mode_str = "1P MODE";
  const char *vs_mode_str = "VS MODE";
  const SDL_Rect p1_str_dst = {250, 298, 112, 26};
  const SDL_Rect vs_str_dst = {250, 348, 112, 26};
  switch (game_count_) {
    case 0: {
      wipe_->set_wipe_in();
      wipe_->draw(screen::width);
      ++game_count_;
      break;
    }
    case 1: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);
      wipe_->draw(screen::width);
      if (wipe_->update()) {
        ++game_count_;
      }
      break;
    }
    case 2: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);
      if (blink_count_ < 30) {
        draw_text(font_size::x16, rgb::black, Point{205, 300},
                  "P r e s s   S p a c e   K e y");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }

      if (input_manager_->edge_key_p(player_type::p1, input_device::x)
          || input_manager_->edge_key_p(player_type::p2, input_device::x)
          || input_manager_->edge_key_p(player_type::p1, input_device::space)) {
        ++game_count_;
        blink_count_ = 0;
      }
      break;
    }
    case 3: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);
      if (!input_manager_->press_key_p(player_type::p1, input_device::x)
          && !input_manager_->press_key_p(player_type::p2, input_device::x)
          && !input_manager_->press_key_p(player_type::p1, input_device::space)) {
        ++game_count_;
      }
      break;
    }
    case 4: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);

      switch (game_mode_) {
        case game_mode::single: {
          SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
          SDL_RenderFillRect(renderer_, &p1_str_dst);
          draw_text(font_size::x16, rgb::white, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::black, vs_mode_pos, vs_mode_str);
          break;
        }
        case game_mode::battle: {
          SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
          SDL_RenderFillRect(renderer_, &vs_str_dst);
          draw_text(font_size::x16, rgb::black, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::white, vs_mode_pos, vs_mode_str);
          break;
        }
      }

      if (input_manager_->press_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->press_key_p(player_type::p1, input_device::space)) {
        wipe_->set_wipe_out();
        wipe_->draw(screen::width);
        ++game_count_;
      }

      if (input_manager_->press_key_p(player_type::p1, input_device::button_2)
          || input_manager_->press_key_p(player_type::p2, input_device::button_2)) {
        game_count_ -= 2;
        game_mode_ = game_mode::single;
      }

      if (input_manager_->press_key_p(player_type::p1, input_device::down)
          || input_manager_->press_key_p(player_type::p2, input_device::down)) {
        game_mode_ = game_mode::battle;
      } else if (input_manager_->press_key_p(player_type::p1, input_device::up)
                 || input_manager_->press_key_p(player_type::p2,
                                                input_device::up)) {
        game_mode_ = game_mode::single;
      }
      break;
    }
    case 5: {
      switch (game_mode_) {
        case game_mode::single: {
          SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
          SDL_RenderFillRect(renderer_, &p1_str_dst);
          draw_text(font_size::x16, rgb::white, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::black, vs_mode_pos, vs_mode_str);
          break;
        }
        case game_mode::battle: {
          SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
          SDL_RenderFillRect(renderer_, &vs_str_dst);
          draw_text(font_size::x16, rgb::black, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::white, vs_mode_pos, vs_mode_str);
          break;
        }
      }

      wipe_->draw(screen::width);

      // initialize globals
      if (wipe_->update()) {
        map_->init(game_mode_);
        food_->init(*map_);
        enemy_->init();
        p1_->init_pos();
        p2_->init_pos();
        p1_->set_life(2);
        p2_->set_life(2);
        p1_->set_score(0);
        p2_->set_score(0);
        p1_->set_damaged(false);
        p2_->set_damaged(false);
        p1_->set_power_mode(0);
        p2_->set_power_mode(0);

        game_count_ = 0;
        game_state_ = game_state::start;
        game_level_ = 1;

        srand((unsigned int)time(nullptr));

        Mix_PlayMusic(mixer_manager_->get_music(), -1);
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void Pacman::game_start() noexcept {
  map_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();
  switch (game_count_) {
    case 0: {
      // TODO: Is it correct?
      if (p1_->get_life() == 2 && p2_->get_life() == 2) {
        Mix_PlayChannel(se_type::beginning,
                        mixer_manager_->get_se(se_type::beginning),
                        0);
      }
      wipe_->set_wipe_in();
      wipe_->draw(screen::offset_x);
      ++game_count_;
      break;
    }
    case 1: {
      wipe_->draw(screen::offset_x);
      if (wipe_->update()) {
        ++game_count_;
      }
      break;
    }
    default:
      ++game_count_;
      break;
  }
  if (game_count_ < 130) {
    std::stringstream ss;
    ss << "S t a g e " << game_level_;
    draw_text(font_size::x36, rgb::red, Point{153, 170}, ss.str().c_str());
  } else if (game_count_ < 200) {
    draw_text(font_size::x36, rgb::red, Point{165, 170}, "S t a r t");
  }

  if (game_count_ > 220) {
    game_count_ = 0;
    game_state_ = game_state::playing;
    p1_->set_power_mode(0);
    p2_->set_power_mode(0);
  }
}

void Pacman::play_game() noexcept {
  map_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();
  enemy_->move(debug_lose_enemy_, *map_, *p1_, *p2_);
  p1_->move(*map_, game_mode_);
  p2_->move(*map_, game_mode_);
  if (p1_->get_power_mode()) {
    p1_->set_power_mode(p1_->get_power_mode() - 1);
  }
  if (p2_->get_power_mode()) {
    p2_->set_power_mode(p2_->get_power_mode() - 1);
  }

  // すべてのエサ取得と敵衝突が同時なら，すべてのエサ取得を優先しクリアへ
  const bool food_state =
      food_->check_state(game_mode_, *p1_, *p2_);
  const bool hit_enemy = enemy_->check_hit_enemy(game_mode_, *p1_, *p2_);
  if (food_state) {
    game_state_ = game_state::clear;
  } else if (hit_enemy) {
    game_state_ = game_state::miss;
  }

  if (input_manager_->edge_key_p(player_type::p1, input_device::space)) {
    game_state_ = game_state::pause;
  }

  if (input_manager_->edge_key_p(player_type::p1, input_device::b)) {
    debug_lose_enemy_ = !debug_lose_enemy_;
  }
}

void Pacman::game_clear() noexcept {
  map_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();

  if (game_count_ == 0) {
    wipe_->set_wipe_out();
    wipe_->draw(screen::offset_x);
    ++game_count_;
    return;
  }

  wipe_->draw(screen::offset_x);
  if (wipe_->update()) {
    if (game_level_ >= 256) {
      game_count_ = 0;
      game_state_ = game_state::gameover;
    } else {
      game_count_ = 0;
      game_state_ = game_state::start;
      ++game_level_;
      food_->init(*map_);
      enemy_->init();
      p1_->init_pos();
      p2_->init_pos();
    }
  }
}

void Pacman::game_miss() noexcept {
  map_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();

  if (game_count_ == 0) {
    Mix_HaltChannel(-1);
    Mix_PlayChannel(-1, mixer_manager_->get_se(se_type::death), 0);
    wipe_->set_wipe_out();
    if (p1_->get_life() == 0 || p2_->get_life() == 0) {
      wipe_->draw(screen::width);
    } else {
      wipe_->draw(screen::offset_x);
    }
    ++game_count_;
    return;
  }

  if (p1_->get_life() == 0 || p2_->get_life() == 0) {
    wipe_->draw(screen::width);
  } else {
    wipe_->draw(screen::offset_x);
  }

  Player *p = p1_->get_damaged() ? p1_.get() : p2_.get();
  const Point pos = p->get_pos();
  p->set_pos(Point{pos.x, pos.y - 1});
  if (wipe_->update()) {
    const int life = p->get_life() - 1;
    p->set_life(life);
    if (life >= 0) {
      game_count_ = 0;
      game_state_ = game_state::start;
      enemy_->init();
      p1_->init_pos();
      p2_->init_pos();
      p1_->set_damaged(false);
      p2_->set_damaged(false);
    } else {
      game_count_ = 0;
      blink_count_ = 0;
      game_state_ = game_state::gameover;
    }
  }
}

void Pacman::game_over() noexcept {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);

  const Point gameover_pos = Point{165, 100};
  const char *gameover_str = "G a m e O v e r";
  switch (game_mode_) {
    case game_mode::single: {
      switch (game_count_) {
        case 0: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_->set_wipe_in();
          wipe_->draw(screen::width);
          ++game_count_;
          break;
        }
        case 1: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_->draw(screen::width);
          if (wipe_->update()) {
            ++game_count_;
          }
          break;
        }
        case 2: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          std::stringstream ss;
          ss << "Y o u r  S c o r e   " << p1_->get_score();
          draw_text(font_size::x36, rgb::black, Point{120, 220},
                    ss.str().c_str());

          if (blink_count_ < 30) {
            draw_text(font_size::x16, rgb::black, Point{210, 350},
                      "P r e s s  S p a c e  K e y");
            ++blink_count_;
          } else if (blink_count_ < 60) {
            ++blink_count_;
          } else {
            blink_count_ = 0;
          }

          if (input_manager_->press_key_p(player_type::p1, input_device::x)
              || input_manager_->press_key_p(player_type::p2, input_device::x)
              || input_manager_->press_key_p(player_type::p1, input_device::space)) {
            ++game_count_;
            wipe_->set_wipe_out();
            wipe_->draw(screen::width);
          }
          break;
        }
        case 3: {
          wipe_->draw(screen::width);
          if (wipe_->update()) {
            blink_count_ = 0;
            game_count_ = 0;
            game_state_ = game_state::title;
            Mix_HaltMusic();
          }
          break;
        }
        default:
          // NOTREACHED
          break;
      }
      break;
    }
    case game_mode::battle: {
      switch (game_count_) {
        case 0: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_->set_wipe_in();
          wipe_->draw(screen::width);
          ++game_count_;
          break;
        }
        case 1: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_->draw(screen::width);
          if (wipe_->update()) {
            ++game_count_;
          }
          break;
        }
        case 2: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          std::stringstream ss;
          const unsigned int p1_score = p1_->get_score();
          const unsigned int p2_score = p2_->get_score();
          if (p1_score > p2_score) {
            ss << "1 P  W I N  " << p1_score;
            draw_text(font_size::x36, rgb::black, Point{170, 240},
                      ss.str().c_str());
          } else if (p1_score < p2_score) {
            ss << "2 P  W I N  " << p2_score;
            draw_text(font_size::x36, rgb::black, Point{170, 240},
                      ss.str().c_str());
          } else {
            ss << "D R A W  " << p1_score;
            draw_text(font_size::x36, rgb::black, Point{170, 240},
                      ss.str().c_str());
          }

          if (blink_count_ < 30) {
            draw_text(font_size::x16, rgb::black, Point{210, 380},
                      "P r e s s  S p a c e  K e y");
            ++blink_count_;
          } else if (blink_count_ < 60) {
            ++blink_count_;
          } else {
            blink_count_ = 0;
          }

          if (input_manager_->press_key_p(player_type::p1, input_device::x)
              || input_manager_->press_key_p(player_type::p2, input_device::x)
              || input_manager_->press_key_p(player_type::p1, input_device::space)) {
            ++game_count_;
            wipe_->set_wipe_out();
            wipe_->draw(screen::width);
          }
          break;
        }
        case 3: {
          wipe_->draw(screen::width);
          if (wipe_->update()) {
            blink_count_ = 0;
            game_count_ = 0;
            game_state_ = game_state::title;
          }
          break;
        }
        default:
          // NOTREACHED
          break;
      }
      break;
    }
  }
}
