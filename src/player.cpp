#include "player.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"

// TODO: set player_type as enum class

void Player::init_pos() noexcept {
  switch (player_type_) {
    case 0: {
      pos_ = {block::size * 9, block::size * 18};
      block_ = {9, 18};
      next_block_ = {9, 18};
      dir_ = 1;
      anime_count_ = 0;
      anime_weight_ = 0;
      return;
    }
    case 1: {
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

void Player::draw() const noexcept {
  switch (player_type_) {
    case 0: {
      SDL_Surface *p_surface = Image_manager::get("player1");
      SDL_Rect src;
      src.x = block::size * dir_;
      src.y = block::size * anime_count_;
      src.w = block::size;
      src.h = block::size;
      SDL_Rect dst;
      dst.x = pos_.x;
      dst.y = pos_.y;
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      return;
    }
    case 1: {
      if (Game_mode != game_mode::battle) {
        return;
      }
      SDL_Surface *p_surface = Image_manager::get("player2");
      SDL_Rect src;
      src.x = block::size * dir_;
      src.y = block::size * anime_count_;
      src.w = block::size;
      src.h = block::size;
      SDL_Rect dst;
      dst.x = pos_.x;
      dst.y = pos_.y;
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      return;
    }
    default:
      // NOTREACHED
      return;
  }
}

// TODO: reduce magic numbers
void Player::move() noexcept {
  if ((player_type_ == 1) && (Game_mode != game_mode::battle)) {
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
  if (Press_key[player_type_][input_device::down]) {
    dir_ = 0;
    ++mut_dst_block.y;
  } else if (Press_key[player_type_][input_device::left]) {
    dir_ = 1;
    --mut_dst_block.x;
  } else if (Press_key[player_type_][input_device::up]) {
    dir_ = 2;
    --mut_dst_block.y;
  } else if (Press_key[player_type_][input_device::right]) {
    dir_ = 3;
    ++mut_dst_block.x;
  }
  const Point dst_block = mut_dst_block;

  const unsigned int dst_block_state = Map::check_state(dst_block);
  const unsigned int dst_right_block_state =
      Map::check_state(Point{dst_block.x + 1, dst_block.y});
  const unsigned int dst_left_block_state =
      Map::check_state(Point{dst_block.x - 1, dst_block.y});
  if ((dst_block_state == 0) || (dst_block_state == 3) ||
      (dst_block_state == 4) || (dst_block_state == 5) ||
      (dst_block_state == 6) || (dst_right_block_state == 6) ||
      (dst_block_state == 7) || (dst_left_block_state == 7) ||
      (dst_block_state == 8)) {
    next_block_ = dst_block;
  }

  // Circle corner
  if (Map::check_state(Point{dst_block.x + 2, dst_block.y}) == 6) {
    next_block_.x = block::count_x;
    pos_.x = block::size * next_block_.x;
  }
  if (Map::check_state(Point{dst_block.x - 2, dst_block.y}) == 7) {
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
