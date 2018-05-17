#ifndef PLAYER_H
#define PLAYER_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "map.hpp"

namespace player_type {

enum {
  p1 = 0,
  p2 = 1,
};

}  // namespace player_type

class Player {
  const unsigned char player_type_;
  Point pos_;
  Point block_;
  Point next_block_;
  unsigned char dir_;           // 0: down, 1: left, 2: up, 3: right
  unsigned char anime_count_;   // 0 or 1
  unsigned char anime_weight_;  // max value is 4
  int life_;
  unsigned int score_;
  bool damaged_p_;
  unsigned int power_mode_;  // 0: not power mode, not 0: power mode

 public:
  Player(const unsigned char player_type) : player_type_(player_type) {}

  inline void init_pos() noexcept {
    switch (player_type_) {
      case player_type::p1: {
        pos_ = {block::size * 9, block::size * 18};
        block_ = {9, 18};
        next_block_ = {9, 18};
        dir_ = 1;
        anime_count_ = 0;
        anime_weight_ = 0;
        return;
      }
      case player_type::p2: {
        pos_ = {block::size * 14, block::size * 18};
        block_ = {14, 18};
        next_block_ = {14, 18};
        dir_ = 3;
        anime_count_ = 0;
        anime_weight_ = 0;
        return;
      }
      default:
        // NOTREACHED
        return;
    }
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager,
                   const game_mode mode) const noexcept {
    switch (player_type_) {
      case player_type::p1: {
        SDL_Surface *p_surface = image_manager.get(image::p1);
        SDL_Rect src = {static_cast<Sint16>(block::size * dir_),
                        static_cast<Sint16>(block::size * anime_count_),
                        block::size, block::size};
        SDL_Rect dst = {static_cast<Sint16>(pos_.x),
                        static_cast<Sint16>(pos_.y), 0, 0};
        SDL_BlitSurface(p_surface, &src, screen, &dst);
        return;
      }
      case player_type::p2: {
        if (mode != game_mode::battle) {
          return;
        }
        SDL_Surface *p_surface = image_manager.get(image::p2);
        SDL_Rect src = {static_cast<Sint16>(block::size * dir_),
                        static_cast<Sint16>(block::size * anime_count_),
                        block::size, block::size};
        SDL_Rect dst = {static_cast<Sint16>(pos_.x),
                        static_cast<Sint16>(pos_.y), 0, 0};
        SDL_BlitSurface(p_surface, &src, screen, &dst);
        return;
      }
      default:
        // NOTREACHED
        return;
    }
  }

  void move(const InputManager &input_manager, const Map &map,
            const game_mode mode) noexcept;

  inline Point get_pos() const noexcept { return pos_; }

  inline void set_pos(const Point &p) noexcept { pos_ = p; }
  inline void set_pos(const Point &&p) noexcept { pos_ = p; }

  inline Point get_block() const noexcept { return block_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  inline unsigned int get_score() const noexcept { return score_; }

  inline void set_score(const unsigned int score) noexcept { score_ = score; }

  inline bool get_damaged() const noexcept { return damaged_p_; }

  inline void set_damaged(const bool damaged_p) noexcept {
    damaged_p_ = damaged_p;
  }

  inline unsigned int get_power_mode() const noexcept { return power_mode_; }

  inline void set_power_mode(const unsigned int power_mode) noexcept {
    power_mode_ = power_mode;
  }

  ~Player() {}
};

#endif
