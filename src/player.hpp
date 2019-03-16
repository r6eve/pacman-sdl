//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

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
  const unsigned char type_;
  const ImageManager *image_manager_;
  const InputManager *input_manager_;
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
  Player(const unsigned char player_type, const ImageManager *image_manager,
         const InputManager *input_manager) noexcept
      : type_(player_type),
        image_manager_(image_manager),
        input_manager_(input_manager) {}

  inline void init_pos() noexcept {
    switch (type_) {
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
    }
  }

  inline void draw(const game_mode mode) const noexcept {
    switch (type_) {
      case player_type::p1: {
        SDL_Texture *p_texture = image_manager_->get(image::p1);
        const SDL_Rect src = {static_cast<Sint16>(block::size * dir_),
                              static_cast<Sint16>(block::size * anime_count_),
                              block::size,
                              block::size};
        const SDL_Rect dst = {static_cast<Sint16>(pos_.x),
                              static_cast<Sint16>(pos_.y),
                              block::size,
                              block::size};
        image_manager_->render_copy(*p_texture, src, dst);
        SDL_DestroyTexture(p_texture);
        return;
      }
      case player_type::p2: {
        if (mode != game_mode::battle) {
          return;
        }
        SDL_Texture *p_texture = image_manager_->get(image::p2);
        const SDL_Rect src = {static_cast<Sint16>(block::size * dir_),
                              static_cast<Sint16>(block::size * anime_count_),
                              block::size,
                              block::size};
        const SDL_Rect dst = {static_cast<Sint16>(pos_.x),
                              static_cast<Sint16>(pos_.y),
                              block::size,
                              block::size};
        image_manager_->render_copy(*p_texture, src, dst);
        SDL_DestroyTexture(p_texture);
        return;
      }
    }
  }

  inline void move(const Map &map, const game_mode mode) noexcept {
    if (type_ == player_type::p2 && mode != game_mode::battle) {
      return;
    }

    const Point dst_pos = {next_block_.x * block::size,
                           next_block_.y * block::size};
    if (pos_.x != dst_pos.x || pos_.y != dst_pos.y) {
      ++anime_weight_;
      if (anime_weight_ > 4) {
        anime_count_ = 1 - anime_count_;
        anime_weight_ = 0;
      }
      const unsigned int move_value = 2;
      if (dst_pos.x > pos_.x) {
        pos_.x += move_value;
      }
      if (dst_pos.y > pos_.y) {
        pos_.y += move_value;
      }
      if (dst_pos.x < pos_.x) {
        pos_.x -= move_value;
      }
      if (dst_pos.y < pos_.y) {
        pos_.y -= move_value;
      }
      return;
    }

    block_ = next_block_;

    // The button precedence is Down > Left > Up > Right.
    Point mut_dst_block = next_block_;
    if (input_manager_->press_key_p(type_, input_device::down)) {
      dir_ = 0;
      ++mut_dst_block.y;
    } else if (input_manager_->press_key_p(type_, input_device::left)) {
      dir_ = 1;
      --mut_dst_block.x;
    } else if (input_manager_->press_key_p(type_, input_device::up)) {
      dir_ = 2;
      --mut_dst_block.y;
    } else if (input_manager_->press_key_p(type_, input_device::right)) {
      dir_ = 3;
      ++mut_dst_block.x;
    }
    const Point dst_block = mut_dst_block;

    const map_state dst_block_state = map.check_state(dst_block);
    const map_state dst_right_block_state =
        map.check_state(Point{dst_block.x + 1, dst_block.y});
    const map_state dst_left_block_state =
        map.check_state(Point{dst_block.x - 1, dst_block.y});
    // TODO: make private function
    if (dst_block_state == map_state::food
        || dst_block_state == map_state::init_p1_pos
        || dst_block_state == map_state::init_p2_pos
        || dst_block_state == map_state::counter_food
        || dst_block_state == map_state::warp_street
        || dst_block_state == map_state::left_warp_pos
        || dst_right_block_state == map_state::left_warp_pos
        || dst_block_state == map_state::right_warp_pos
        || dst_left_block_state == map_state::right_warp_pos) {
      next_block_ = dst_block;
    }

    // Circle corner
    if (map.check_state(Point{dst_block.x + 2, dst_block.y})
        == map_state::left_warp_pos) {
      next_block_.x = block::count_x;
      pos_.x = block::size * next_block_.x;
    }
    if (map.check_state(Point{dst_block.x - 2, dst_block.y})
        == map_state::right_warp_pos) {
      next_block_.x = -1;
      pos_.x = block::size * next_block_.x;
    }
  }

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

  ~Player() noexcept {}
};
