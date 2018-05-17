#include "player.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "map.hpp"

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
