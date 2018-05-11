#include "player.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "map.hpp"

// TODO: set player_type as enum class

void Player::init_pos(const unsigned int player_type) noexcept {
  switch (player_type) {
    case 0: {
      pos_x_ = block::size * 9;
      pos_y_ = block::size * 18;
      block_x_ = 9;
      block_y_ = 18;
      next_block_x_ = 9;
      next_block_y_ = 18;
      dir_ = 1;
      anime_count_ = 0;
      anime_weight_ = 0;
      break;
    }
    case 1: {
      pos_x_ = block::size * 14;
      pos_y_ = block::size * 18;
      block_x_ = 14;
      block_y_ = 18;
      next_block_x_ = 14;
      next_block_y_ = 18;
      dir_ = 3;
      anime_count_ = 0;
      anime_weight_ = 0;
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void Player::draw(const unsigned int player_type) const noexcept {
  switch (player_type) {
    case 0: {
      SDL_Surface *p_surface = Image_manager::get("player1");
      SDL_Rect src;
      src.x = block::size * dir_;
      src.y = block::size * anime_count_;
      src.w = block::size;
      src.h = block::size;
      SDL_Rect dst;
      dst.x = pos_x_;
      dst.y = pos_y_;
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      break;
    }
    case 1: {
      SDL_Surface *p_surface = Image_manager::get("player2");
      SDL_Rect src;
      src.x = block::size * dir_;
      src.y = block::size * anime_count_;
      src.w = block::size;
      src.h = block::size;
      SDL_Rect dst;
      dst.x = pos_x_;
      dst.y = pos_y_;
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

// TODO: reduce magic numbers
void Player::move(const unsigned int player_type) noexcept {
  const int dst_pos_x = next_block_x_ * block::size;
  const int dst_pos_y = next_block_y_ * block::size;
  if ((pos_x_ != dst_pos_x) || (pos_y_ != dst_pos_y)) {
    ++anime_weight_;
    if (anime_weight_ > 4) {
      anime_count_ = 1 - anime_count_;
      anime_weight_ = 0;
    }
    const unsigned int move_value = 2;
    if (dst_pos_x > pos_x_) {
      pos_x_ += move_value;
    }
    if (dst_pos_y > pos_y_) {
      pos_y_ += move_value;
    }
    if (dst_pos_x < pos_x_) {
      pos_x_ -= move_value;
    }
    if (dst_pos_y < pos_y_) {
      pos_y_ -= move_value;
    }
    return;
  }

  block_x_ = next_block_x_;
  block_y_ = next_block_y_;

  // 同時押しの場合，優先順位は
  // Down > Left > Up > Right
  int mut_dst_block_x = next_block_x_;
  int mut_dst_block_y = next_block_y_;
  if (Press_key[player_type][input_device::down]) {
    dir_ = 0;
    ++mut_dst_block_y;
  } else if (Press_key[player_type][input_device::left]) {
    dir_ = 1;
    --mut_dst_block_x;
  } else if (Press_key[player_type][input_device::up]) {
    dir_ = 2;
    --mut_dst_block_y;
  } else if (Press_key[player_type][input_device::right]) {
    dir_ = 3;
    ++mut_dst_block_x;
  }
  const int dst_block_x = mut_dst_block_x;
  const int dst_block_y = mut_dst_block_y;

  const unsigned int dst_block_state =
      Map::check_state(dst_block_x, dst_block_y);
  const unsigned int dst_right_block_state =
      Map::check_state(dst_block_x + 1, dst_block_y);
  const unsigned int dst_left_block_state =
      Map::check_state(dst_block_x - 1, dst_block_y);
  if ((dst_block_state == 0) || (dst_block_state == 3) ||
      (dst_block_state == 4) || (dst_block_state == 5) ||
      (dst_block_state == 6) || (dst_right_block_state == 6) ||
      (dst_block_state == 7) || (dst_left_block_state == 7) ||
      (dst_block_state == 8)) {
    next_block_x_ = dst_block_x;
    next_block_y_ = dst_block_y;
  }

  // Circle corner
  if (Map::check_state(dst_block_x + 2, dst_block_y) == 6) {
    next_block_x_ = block::count_x;
    pos_x_ = block::size * next_block_x_;
  }
  if (Map::check_state(dst_block_x - 2, dst_block_y) == 7) {
    next_block_x_ = -1;
    pos_x_ = block::size * next_block_x_;
  }
}

void Player::add_pos(int x, int y) noexcept {
  pos_x_ += x;
  pos_y_ += y;
}

int Player::get_pos_x() const noexcept { return pos_x_; }

int Player::get_pos_y() const noexcept { return pos_y_; }

int Player::get_block_x() const noexcept { return block_x_; }

int Player::get_block_y() const noexcept { return block_y_; }

int Player::get_life() const noexcept { return life_; }

void Player::set_life(const int life) noexcept { life_ = life; }
