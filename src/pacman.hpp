#ifndef PACMAN_H
#define PACMAN_H

#include "enemy.hpp"
#include "food.hpp"
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

  const bool debug_mode_;

  game_state game_state_;
  unsigned int game_level_;
  game_mode game_mode_;
  unsigned int blink_count_;
  unsigned int game_count_;
  bool debug_lose_enemy_;

  void init() noexcept;
  void init_sdl();
  void main_loop() noexcept;
  void game_title(Wipe &wipe, Food &food, Enemy &enemy, Player &p1,
                  Player &p2) noexcept;
  void game_start(Wipe &wipe, Food &food, const Enemy &enemy, Player &p1,
                  Player &p2) noexcept;
  void play_game(Food &food, Enemy &enemy, Player &p1, Player &p2) noexcept;
  void game_clear(Wipe &wipe, Food &food, Enemy &enemy, Player &p1,
                  Player &p2) noexcept;
  void game_miss(Wipe &wipe, Food &food, Enemy &enemy, Player &p1,
                 Player &p2) noexcept;
  void game_over(Wipe &wipe, const Player &p1, const Player &p2) noexcept;
  void game_pause(Food &food, const Enemy &enemy, Player &p1,
                  Player &p2) noexcept;
  // TODO: make enum class `font_type` and `color`
  void draw_text(const unsigned char font_size, Uint8 r, Uint8 g, Uint8 b,
                 int x, int y, const char *str) noexcept;
  void draw_score(Player &p1, Player &p2) noexcept;
  bool poll_event() noexcept;
  void wait_game() noexcept;
  void draw_fps() noexcept;
  void end() noexcept;
  void draw_translucence() noexcept;

 public:
  Pacman(const bool debug_mode) : debug_mode_(debug_mode) {}
  void run() noexcept;
  ~Pacman() {}
};

#endif
