#include "player.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "map.hpp"

void Player::init_pos() noexcept {
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

void Player::draw(SDL_Surface *screen, const ImageManager &image_manager,
                  const game_mode mode) const noexcept {
  switch (player_type_) {
    case player_type::p1: {
      SDL_Surface *p_surface = image_manager.get(image::p1);
      SDL_Rect src;
      src.x = block::size * dir_;
      src.y = block::size * anime_count_;
      src.w = block::size;
      src.h = block::size;
      SDL_Rect dst;
      dst.x = pos_.x;
      dst.y = pos_.y;
      SDL_BlitSurface(p_surface, &src, screen, &dst);
      return;
    }
    case player_type::p2: {
      if (mode != game_mode::battle) {
        return;
      }
      SDL_Surface *p_surface = image_manager.get(image::p2);
      SDL_Rect src;
      src.x = block::size * dir_;
      src.y = block::size * anime_count_;
      src.w = block::size;
      src.h = block::size;
      SDL_Rect dst;
      dst.x = pos_.x;
      dst.y = pos_.y;
      SDL_BlitSurface(p_surface, &src, screen, &dst);
      return;
    }
    default:
      // NOTREACHED
      return;
  }
}

void Player::move(const InputManager &input_manager, const Map &map,
                  const game_mode mode) noexcept {
  if ((player_type_ == player_type::p2) && (mode != game_mode::battle)) {
    return;
  }

  const Point dst_pos = {next_block_.x * block::size,
                         next_block_.y * block::size};
  if ((pos_.x != dst_pos.x) || (pos_.y != dst_pos.y)) {
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

  // 同時押しの場合，優先順位は Down > Left > Up > Right
  Point mut_dst_block = next_block_;
  if (input_manager.press_key_p(player_type_, input_device::down)) {
    dir_ = 0;
    ++mut_dst_block.y;
  } else if (input_manager.press_key_p(player_type_, input_device::left)) {
    dir_ = 1;
    --mut_dst_block.x;
  } else if (input_manager.press_key_p(player_type_, input_device::up)) {
    dir_ = 2;
    --mut_dst_block.y;
  } else if (input_manager.press_key_p(player_type_, input_device::right)) {
    dir_ = 3;
    ++mut_dst_block.x;
  }
  const Point dst_block = mut_dst_block;

  const map_state dst_block_state = map.check_state(dst_block);
  const map_state dst_right_block_state =
      map.check_state(Point{dst_block.x + 1, dst_block.y});
  const map_state dst_left_block_state =
      map.check_state(Point{dst_block.x - 1, dst_block.y});
  if ((dst_block_state == map_state::food) ||
      (dst_block_state == map_state::init_p1_pos) ||
      (dst_block_state == map_state::init_p2_pos) ||
      (dst_block_state == map_state::counter_food) ||
      (dst_block_state == map_state::warp_street) ||
      (dst_block_state == map_state::left_warp_pos) ||
      (dst_right_block_state == map_state::left_warp_pos) ||
      (dst_block_state == map_state::right_warp_pos) ||
      (dst_left_block_state == map_state::right_warp_pos)) {
    next_block_ = dst_block;
  }

  // Circle corner
  if (map.check_state(Point{dst_block.x + 2, dst_block.y}) ==
      map_state::left_warp_pos) {
    next_block_.x = block::count_x;
    pos_.x = block::size * next_block_.x;
  }
  if (map.check_state(Point{dst_block.x - 2, dst_block.y}) ==
      map_state::right_warp_pos) {
    next_block_.x = -1;
    pos_.x = block::size * next_block_.x;
  }
}

Point Player::get_pos() const noexcept { return pos_; }

void Player::set_pos(const Point &p) noexcept { pos_ = p; }

void Player::set_pos(const Point &&p) noexcept { pos_ = p; }

Point Player::get_block() const noexcept { return block_; }

int Player::get_life() const noexcept { return life_; }

void Player::set_life(const int life) noexcept { life_ = life; }

unsigned int Player::get_score() const noexcept { return score_; }

void Player::set_score(const unsigned int score) noexcept { score_ = score; }

bool Player::get_damaged() const noexcept { return damaged_p_; }

void Player::set_damaged(const bool damaged_p) noexcept {
  damaged_p_ = damaged_p;
}

unsigned int Player::get_power_mode() const noexcept { return power_mode_; }

void Player::set_power_mode(const unsigned int power_mode) noexcept {
  power_mode_ = power_mode;
}
