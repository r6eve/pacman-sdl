#ifndef PACMAN_H
#define PACMAN_H

#include <memory>
#include "enemy.hpp"
#include "font_manager.hpp"
#include "food.hpp"
#include "input_manager.hpp"
#include "map.hpp"
#include "player.hpp"
#include "wipe.hpp"

class Pacman {
  enum class game_state {
    title,
    start,
    clear,
    miss,
    playing,
    gameover,
    pause,
  };

  struct RGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
  };

  const bool debug_mode_;

  SDL_Surface *screen_;
  game_state game_state_;
  unsigned int game_level_;
  game_mode game_mode_;
  unsigned int blink_count_;
  unsigned int game_count_;
  bool debug_lose_enemy_;

  Wipe wipe_;
  Map map_;
  Food food_;
  Enemy enemy_;
  Player p1_;
  Player p2_;
  FontManager font_manager_;
  ImageManager image_manager_;
  InputManager input_manager_;

  void init_sdl();
  void game_title() noexcept;
  void game_start() noexcept;
  void play_game() noexcept;
  void game_clear() noexcept;
  void game_miss() noexcept;
  void game_over() noexcept;
  void game_pause() noexcept;

  void draw_text(const unsigned char font_size, const RGB &rgb,
                 const Point &p, const char *str) const noexcept;
  void draw_text(const unsigned char font_size, const RGB &&rgb,
                 const Point &p, const char *str) const noexcept;
  void draw_text(const unsigned char font_size, const RGB &rgb,
                 const Point &&p, const char *str) const noexcept;
  void draw_text(const unsigned char font_size, const RGB &&rgb,
                 const Point &&p, const char *str) const noexcept;

  void draw_score() noexcept;
  bool poll_event() const noexcept;
  void wait_game() const noexcept;
  void draw_fps() const noexcept;
  void draw_translucence() noexcept;

 public:
  Pacman(const bool debug_mode) noexcept;
  void run() noexcept;
  ~Pacman() noexcept;
};

#endif
