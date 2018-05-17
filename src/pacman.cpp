#define PACMAN_CPP

#include "pacman.hpp"
#include <SDL/SDL_mixer.h>
#include <time.h>
#include <iomanip>
#include <iostream>
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

using namespace std;

Pacman::Pacman(const bool debug_mode) noexcept
    : debug_mode_(debug_mode),
      screen_(nullptr),
      game_state_(game_state::title),
      game_mode_(game_mode::single),
      blink_count_(0),
      game_count_(0),
      debug_lose_enemy_(false),
      p1_(player_type::p1),
      p2_(player_type::p2) {
  try {
    init_sdl();
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    exit(EXIT_FAILURE);
  }
}

void Pacman::init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw SDL_GetError();
  }

  SDL_WM_SetCaption("pacman-sdl", nullptr);
  if (debug_mode_) {
    screen_ = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                               SDL_HWSURFACE | SDL_DOUBLEBUF);
  } else {
    screen_ = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                               SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  }
  if (!screen_) {
    throw SDL_GetError();
  }

  SDL_ShowCursor(SDL_DISABLE);
}

void Pacman::run() noexcept {
  for (;;) {
    input_manager_.update(debug_mode_);
    switch (game_state_) {
      case game_state::title:
        game_title();
        break;
      case game_state::start:
        game_start();
        break;
      case game_state::playing:
        play_game();
        break;
      case game_state::clear:
        game_clear();
        break;
      case game_state::miss:
        game_miss();
        break;
      case game_state::gameover:
        game_over();
        break;
      case game_state::pause:
        game_pause();
        break;
      default:
        // NOTREACHED
        break;
    }
    if (!poll_event()) {
      return;
    }
    if (debug_mode_) {
      draw_fps();
    }
    SDL_Flip(screen_);
    wait_game();
  }
}

void Pacman::game_title() noexcept {
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_FillRect(screen_, &dst, 0xffffffff);

  const Point title_pos = Point{160, 160};
  const Point p1_mode_pos = Point{270, 300};
  const Point vs_mode_pos = Point{270, 350};
  const char *title_str = "P  a  c  -  M  a  n";
  const char *p1_mode_str = "1P MODE";
  const char *vs_mode_str = "VS MODE";
  SDL_Rect p1_str_dst = {250, 298, 112, 26};
  SDL_Rect vs_str_dst = {250, 348, 112, 26};
  switch (game_count_) {
    case 0: {
      wipe_.set_wipe_in();
      wipe_.draw(screen_, screen::width);
      ++game_count_;
      break;
    }
    case 1: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);
      wipe_.draw(screen_, screen::width);
      if (wipe_.update()) {
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

      if (input_manager_.edge_key_p(player_type::p1, input_device::x) ||
          input_manager_.edge_key_p(player_type::p2, input_device::x) ||
          input_manager_.edge_key_p(player_type::p1, input_device::space)) {
        ++game_count_;
        blink_count_ = 0;
      }
      break;
    }
    case 3: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);
      if (!input_manager_.press_key_p(player_type::p1, input_device::x) &&
          !input_manager_.press_key_p(player_type::p2, input_device::x) &&
          !input_manager_.press_key_p(player_type::p1, input_device::space)) {
        ++game_count_;
      }
      break;
    }
    case 4: {
      draw_text(font_size::x36, rgb::black, title_pos, title_str);

      switch (game_mode_) {
        case game_mode::single: {
          SDL_FillRect(screen_, &p1_str_dst, 0x00000000);
          draw_text(font_size::x16, rgb::white, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::black, vs_mode_pos, vs_mode_str);
          break;
        }
        case game_mode::battle: {
          SDL_FillRect(screen_, &vs_str_dst, 0x00000000);
          draw_text(font_size::x16, rgb::black, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::white, vs_mode_pos, vs_mode_str);
          break;
        }
        default:
          // NOTREACHED
          break;
      }

      if (input_manager_.press_key_p(player_type::p1, input_device::x) ||
          input_manager_.press_key_p(player_type::p2, input_device::x) ||
          input_manager_.press_key_p(player_type::p1, input_device::space)) {
        wipe_.set_wipe_out();
        wipe_.draw(screen_, screen::width);
        ++game_count_;
      }

      if (input_manager_.press_key_p(player_type::p1, input_device::button_2) ||
          input_manager_.press_key_p(player_type::p2, input_device::button_2)) {
        game_count_ -= 2;
        game_mode_ = game_mode::single;
      }

      if (input_manager_.press_key_p(player_type::p1, input_device::down) ||
          input_manager_.press_key_p(player_type::p2, input_device::down)) {
        game_mode_ = game_mode::battle;
      } else if (input_manager_.press_key_p(player_type::p1,
                                            input_device::up) ||
                 input_manager_.press_key_p(player_type::p2,
                                            input_device::up)) {
        game_mode_ = game_mode::single;
      }
      break;
    }
    case 5: {
      switch (game_mode_) {
        case game_mode::single: {
          SDL_FillRect(screen_, &p1_str_dst, 0x00000000);
          draw_text(font_size::x16, rgb::white, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::black, vs_mode_pos, vs_mode_str);
          break;
        }
        case game_mode::battle: {
          SDL_FillRect(screen_, &vs_str_dst, 0x00000000);
          draw_text(font_size::x16, rgb::black, p1_mode_pos, p1_mode_str);
          draw_text(font_size::x16, rgb::white, vs_mode_pos, vs_mode_str);
          break;
        }
        default:
          // NOTREACHED
          break;
      }

      wipe_.draw(screen_, screen::width);

      // initialize globals
      if (wipe_.update()) {
        map_.init(game_mode_);
        food_.init(map_);
        enemy_.init();
        p1_.init_pos();
        p2_.init_pos();
        p1_.set_life(2);
        p2_.set_life(2);
        p1_.set_score(0);
        p2_.set_score(0);
        p1_.set_damaged(false);
        p2_.set_damaged(false);
        p1_.set_power_mode(0);
        p2_.set_power_mode(0);

        game_count_ = 0;
        game_state_ = game_state::start;
        game_level_ = 1;

        srand((unsigned int)time(nullptr));
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void Pacman::game_start() noexcept {
  map_.draw(screen_, image_manager_, game_level_);
  food_.draw(screen_, image_manager_);
  enemy_.draw(screen_, image_manager_);
  p1_.draw(screen_, image_manager_, game_mode_);
  p2_.draw(screen_, image_manager_, game_mode_);
  draw_score();
  switch (game_count_) {
    case 0: {
      // TODO: Is it correct?
      if ((p1_.get_life() == 2) && (p2_.get_life() == 2)) {
        Mix_PlayMusic(mixer_manager_.get_music(music_type::beginning), 0);
      }
      wipe_.set_wipe_in();
      wipe_.draw(screen_, screen::offset_x);
      ++game_count_;
      break;
    }
    case 1: {
      wipe_.draw(screen_, screen::offset_x);
      if (wipe_.update()) {
        ++game_count_;
      }
      break;
    }
    default:
      ++game_count_;
      break;
  }
  if (game_count_ < 130) {
    stringstream ss;
    ss << "S t a g e " << game_level_;
    draw_text(font_size::x36, rgb::red, Point{153, 170}, ss.str().c_str());
  } else if (game_count_ < 200) {
    draw_text(font_size::x36, rgb::red, Point{165, 170}, "S t a r t");
  }

  if (game_count_ > 220) {
    game_count_ = 0;
    game_state_ = game_state::playing;
    p1_.set_power_mode(0);
    p2_.set_power_mode(0);
  }
}

void Pacman::play_game() noexcept {
  map_.draw(screen_, image_manager_, game_level_);
  food_.draw(screen_, image_manager_);
  enemy_.draw(screen_, image_manager_);
  p1_.draw(screen_, image_manager_, game_mode_);
  p2_.draw(screen_, image_manager_, game_mode_);
  draw_score();
  enemy_.move(debug_lose_enemy_, map_, p1_, p2_);
  p1_.move(input_manager_, map_, game_mode_);
  p2_.move(input_manager_, map_, game_mode_);
  if (p1_.get_power_mode()) {
    p1_.set_power_mode(p1_.get_power_mode() - 1);
  }
  if (p2_.get_power_mode()) {
    p2_.set_power_mode(p2_.get_power_mode() - 1);
  }

  // すべてのエサ取得と敵衝突が同時なら，すべてのエサ取得を優先しクリアへ
  const bool food_state =
      food_.check_state(game_mode_, mixer_manager_, p1_, p2_);
  const bool hit_enemy = enemy_.check_hit_enemy(game_mode_, p1_, p2_);
  if (food_state) {
    game_state_ = game_state::clear;
  } else if (hit_enemy) {
    game_state_ = game_state::miss;
  }

  if (input_manager_.edge_key_p(player_type::p1, input_device::space)) {
    game_state_ = game_state::pause;
  }

  if (input_manager_.edge_key_p(player_type::p1, input_device::b)) {
    debug_lose_enemy_ = !debug_lose_enemy_;
  }
}

void Pacman::game_clear() noexcept {
  map_.draw(screen_, image_manager_, game_level_);
  food_.draw(screen_, image_manager_);
  enemy_.draw(screen_, image_manager_);
  p1_.draw(screen_, image_manager_, game_mode_);
  p2_.draw(screen_, image_manager_, game_mode_);
  draw_score();

  if (game_count_ == 0) {
    wipe_.set_wipe_out();
    wipe_.draw(screen_, screen::offset_x);
    ++game_count_;
    return;
  }

  wipe_.draw(screen_, screen::offset_x);
  if (wipe_.update()) {
    if (game_level_ >= 256) {
      game_count_ = 0;
      game_state_ = game_state::gameover;
    } else {
      game_count_ = 0;
      game_state_ = game_state::start;
      ++game_level_;
      food_.init(map_);
      enemy_.init();
      p1_.init_pos();
      p2_.init_pos();
    }
  }
}

void Pacman::game_miss() noexcept {
  map_.draw(screen_, image_manager_, game_level_);
  food_.draw(screen_, image_manager_);
  enemy_.draw(screen_, image_manager_);
  p1_.draw(screen_, image_manager_, game_mode_);
  p2_.draw(screen_, image_manager_, game_mode_);
  draw_score();

  if (game_count_ == 0) {
    Mix_PlayMusic(mixer_manager_.get_music(music_type::death), 0);
    wipe_.set_wipe_out();
    if ((p1_.get_life() == 0) || (p2_.get_life() == 0)) {
      wipe_.draw(screen_, screen::width);
    } else {
      wipe_.draw(screen_, screen::offset_x);
    }
    ++game_count_;
    return;
  }

  if ((p1_.get_life() == 0) || (p2_.get_life() == 0)) {
    wipe_.draw(screen_, screen::width);
  } else {
    wipe_.draw(screen_, screen::offset_x);
  }

  // TODO: use pointer to delete if-clauses
  if (p1_.get_damaged()) {
    Point pos = p1_.get_pos();
    p1_.set_pos(Point{pos.x, pos.y - 1});
    if (wipe_.update()) {
      const int life = p1_.get_life() - 1;
      p1_.set_life(life);
      if (life >= 0) {
        game_count_ = 0;
        game_state_ = game_state::start;
        enemy_.init();
        p1_.init_pos();
        p2_.init_pos();
        p1_.set_damaged(false);
        p2_.set_damaged(false);
      } else {
        game_count_ = 0;
        blink_count_ = 0;
        game_state_ = game_state::gameover;
      }
    }
  } else {
    Point pos = p2_.get_pos();
    p2_.set_pos(Point{pos.x, pos.y - 1});
    if (wipe_.update()) {
      const int life = p2_.get_life() - 1;
      p2_.set_life(life);
      if (life >= 0) {
        game_count_ = 0;
        game_state_ = game_state::start;
        enemy_.init();
        p1_.init_pos();
        p2_.init_pos();
        p1_.set_damaged(false);
        p2_.set_damaged(false);
      } else {
        game_count_ = 0;
        blink_count_ = 0;
        game_state_ = game_state::gameover;
      }
    }
  }
}

void Pacman::game_over() noexcept {
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_FillRect(screen_, &dst, 0xffffffff);

  const Point gameover_pos = Point{165, 100};
  const char *gameover_str = "G a m e O v e r";
  switch (game_mode_) {
    case game_mode::single: {
      switch (game_count_) {
        case 0: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_.set_wipe_in();
          wipe_.draw(screen_, screen::width);
          ++game_count_;
          break;
        }
        case 1: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_.draw(screen_, screen::width);
          if (wipe_.update()) {
            ++game_count_;
          }
          break;
        }
        case 2: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          stringstream ss;
          ss << "Y o u r  S c o r e   " << p1_.get_score();
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

          if (input_manager_.press_key_p(player_type::p1, input_device::x) ||
              input_manager_.press_key_p(player_type::p2, input_device::x) ||
              input_manager_.press_key_p(player_type::p1,
                                         input_device::space)) {
            ++game_count_;
            wipe_.set_wipe_out();
            wipe_.draw(screen_, screen::width);
          }
          break;
        }
        case 3: {
          wipe_.draw(screen_, screen::width);
          if (wipe_.update()) {
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
    case game_mode::battle: {
      switch (game_count_) {
        case 0: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_.set_wipe_in();
          wipe_.draw(screen_, screen::width);
          ++game_count_;
          break;
        }
        case 1: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          wipe_.draw(screen_, screen::width);
          if (wipe_.update()) {
            ++game_count_;
          }
          break;
        }
        case 2: {
          draw_text(font_size::x36, rgb::red, gameover_pos, gameover_str);
          stringstream ss;
          const unsigned int p1_score = p1_.get_score();
          const unsigned int p2_score = p2_.get_score();
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

          if (input_manager_.press_key_p(player_type::p1, input_device::x) ||
              input_manager_.press_key_p(player_type::p2, input_device::x) ||
              input_manager_.press_key_p(player_type::p1,
                                         input_device::space)) {
            ++game_count_;
            wipe_.set_wipe_out();
            wipe_.draw(screen_, screen::width);
          }
          break;
        }
        case 3: {
          wipe_.draw(screen_, screen::width);
          if (wipe_.update()) {
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
    default:
      // NOTREACHED
      break;
  }
}

void Pacman::game_pause() noexcept {
  map_.draw(screen_, image_manager_, game_level_);
  food_.draw(screen_, image_manager_);
  enemy_.draw(screen_, image_manager_);
  p1_.draw(screen_, image_manager_, game_mode_);
  p2_.draw(screen_, image_manager_, game_mode_);
  draw_score();
  draw_translucence();
  if (input_manager_.edge_key_p(player_type::p1, input_device::space)) {
    game_state_ = game_state::playing;
  }
}

void Pacman::draw_text(const unsigned char font_size, const RGB &rgb,
                       const Point &p, const char *str) const noexcept {
  const SDL_Color color = {rgb.r, rgb.g, rgb.b, 0};
  SDL_Surface *font_surface =
      TTF_RenderUTF8_Blended(font_manager_.get(font_size), str, color);
  SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),
                  static_cast<Uint16>(font_surface->h)};
  SDL_Rect dst = {static_cast<Sint16>(p.x), static_cast<Sint16>(p.y), 0, 0};
  SDL_BlitSurface(font_surface, &src, screen_, &dst);
}

void Pacman::draw_text(const unsigned char font_size, const RGB &&rgb,
                       const Point &p, const char *str) const noexcept {
  draw_text(font_size, rgb, p, str);
}

void Pacman::draw_text(const unsigned char font_size, const RGB &rgb,
                       const Point &&p, const char *str) const noexcept {
  draw_text(font_size, rgb, p, str);
}

void Pacman::draw_text(const unsigned char font_size, const RGB &&rgb,
                       const Point &&p, const char *str) const noexcept {
  draw_text(font_size, rgb, p, str);
}

void Pacman::draw_score() const noexcept {
  // Draw the plate of background.
  {
    SDL_Surface *p_surface = image_manager_.get(image::plate);
    SDL_Rect dst = {screen::offset_x, 0, 0, 0};
    SDL_BlitSurface(p_surface, nullptr, screen_, &dst);
  }

  // Draw the score itself.
  {
    const unsigned int x1 = screen::offset_x + 20;
    const unsigned int y1 = screen::height / 7 + 10;
    const unsigned int x2 = x1 + 40;
    const unsigned int y2 = y1 + 30;
    const unsigned int x3 = x2 + 30;
    const unsigned int y3 = y2;

    stringstream score;
    score << "S c o r e  :  " << setw(6) << p1_.get_score();
    draw_text(font_size::x16, rgb::white, Point{x1, y1}, score.str().c_str());

    SDL_Surface *p_surface = image_manager_.get(image::p1);
    SDL_Rect src = {block::size, 0, block::size, block::size};
    SDL_Rect dst = {x2, y2, 0, 0};
    SDL_BlitSurface(p_surface, &src, screen_, &dst);

    stringstream life;
    life << "x  " << p1_.get_life();
    draw_text(font_size::x16, rgb::white, Point{x3, y3}, life.str().c_str());

    if (game_mode_ == game_mode::battle) {
      const unsigned int offset_y = 80;
      stringstream score;
      score << "S c o r e  :  " << setw(6) << p2_.get_score();
      draw_text(font_size::x16, rgb::white, Point{x1, y1 + offset_y},
                score.str().c_str());

      SDL_Surface *p_surface = image_manager_.get(image::p2);
      SDL_Rect src = {block::size, 0, block::size, block::size};
      SDL_Rect dst = {x2, y2 + offset_y, 0, 0};
      SDL_BlitSurface(p_surface, &src, screen_, &dst);

      stringstream life;
      life << "x  " << p2_.get_life();
      draw_text(font_size::x16, rgb::white, Point{x3, y3 + offset_y},
                life.str().c_str());
    }
  }

  // Draw the rest time of power mode.
  {
    const unsigned int x = screen::offset_x + 10;
    const unsigned int y = screen::height / 6 * 4;
    if (p1_.get_power_mode()) {
      SDL_Rect dst = {x, y, static_cast<Uint16>(p1_.get_power_mode() / 4),
                      block::size};
      SDL_FillRect(screen_, &dst, 0xffff00);
    }
    if (p2_.get_power_mode()) {
      SDL_Rect dst = {x, y + 30, static_cast<Uint16>(p2_.get_power_mode() / 4),
                      block::size};
      SDL_FillRect(screen_, &dst, 0x808080);
    }
  }
}

bool Pacman::poll_event() const noexcept {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return false;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }
        break;
      default:
        // do nothing
        break;
    }
  }
  return true;
}

void Pacman::wait_game() const noexcept {
  static Uint32 pre_count;
  const double wait_time = 1000.0 / screen::max_fps;
  const Uint32 wait_count = (wait_time + 0.5);
  if (pre_count) {
    const Uint32 now_count = SDL_GetTicks();
    const Uint32 interval = now_count - pre_count;
    if (interval < wait_count) {
      const Uint32 delay_time = wait_count - interval;
      SDL_Delay(delay_time);
    }
  }
  pre_count = SDL_GetTicks();
}

void Pacman::draw_fps() const noexcept {
  static Uint32 pre_count;
  const Uint32 now_count = SDL_GetTicks();
  if (pre_count) {
    static double frame_rate;
    Uint32 mut_interval = now_count - pre_count;
    if (mut_interval < 1) {
      mut_interval = 1;
    }
    const Uint32 interval = mut_interval;

    if (!(pre_count % 30)) {
      frame_rate = 1000.0 / interval;
    }

    stringstream ss;
    ss << "FrameRate[" << setprecision(2) << setiosflags(ios::fixed)
       << frame_rate << "]";
    draw_text(font_size::x16, rgb::white, Point{screen::offset_x + 15, 16},
              ss.str().c_str());
  }
  pre_count = now_count;
}

void Pacman::draw_translucence() noexcept {
  Uint32 rmask, gmask, bmask, amask;
  const Uint8 alpha = 128;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  SDL_Rect dst = {0, 0, 0, 0};
  SDL_Surface *trans_surface =
      SDL_CreateRGBSurface(SDL_SWSURFACE, screen::width, screen::height, 32,
                           rmask, gmask, bmask, amask);
  if (!trans_surface) {
    cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(trans_surface, nullptr, screen_, &dst);
  if (blink_count_ < 30) {
    draw_text(font_size::x36, rgb::white, Point{165, 170}, "P a u s e");
    ++blink_count_;
  } else if (blink_count_ < 60) {
    ++blink_count_;
  } else {
    blink_count_ = 0;
  }
}

Pacman::~Pacman() noexcept { atexit(SDL_Quit); }
