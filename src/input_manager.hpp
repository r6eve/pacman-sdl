//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <vector>
#include <iostream>
#include "def_global.hpp"

// Keyboard and Joystick
namespace input_device {

enum {
  up = 0,
  down,
  left,
  right,
  x,
  c,
  b,  // Debug key: Enemys back home.
  space,

  // used by only Joystick
  button_2,
  button_3,
  button_4,
  button_5,
  button_6,
  button_7,
  button_8,
  button_9,

  start,
  select,
  count,
};

}  // namespace input_device

class InputManager {
  std::vector<SDL_Joystick *> joysticks_;
  bool edge_key_[2][input_device::count];
  bool press_key_[2][input_device::count];

 public:
  InputManager() noexcept {
    const int n = SDL_NumJoysticks();
    if (n < 0) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    if (n == 0) {
      // JoySticks not found.
      return;
    }

    for (int i = 0; i < n; ++i) {
      joysticks_.push_back(SDL_JoystickOpen(i));
    }
  }

  inline void update(const bool debug_mode) noexcept {
    constexpr unsigned int num_devices_ = 2;
    bool new_press_key[num_devices_][input_device::count];
    for (unsigned int i = 0; i < num_devices_; ++i) {
      for (unsigned int j = 0; j < input_device::count; ++j) {
        new_press_key[i][j] = false;
      }
    }

    const unsigned int num_joysticks = joysticks_.size();
    for (unsigned int i = 0; i < num_joysticks; ++i) {
      if (joysticks_[i]) {
        SDL_JoystickUpdate();
        new_press_key[i][input_device::x] =
            SDL_JoystickGetButton(joysticks_[i], 0) == SDL_PRESSED;
        new_press_key[i][input_device::c] =
            SDL_JoystickGetButton(joysticks_[i], 1) == SDL_PRESSED;
        new_press_key[i][input_device::button_2] =
            SDL_JoystickGetButton(joysticks_[i], 2) == SDL_PRESSED;
        new_press_key[i][input_device::button_3] =
            SDL_JoystickGetButton(joysticks_[i], 3) == SDL_PRESSED;
        new_press_key[i][input_device::button_4] =
            SDL_JoystickGetButton(joysticks_[i], 4) == SDL_PRESSED;
        new_press_key[i][input_device::button_5] =
            SDL_JoystickGetButton(joysticks_[i], 5) == SDL_PRESSED;
        new_press_key[i][input_device::button_6] =
            SDL_JoystickGetButton(joysticks_[i], 6) == SDL_PRESSED;
        new_press_key[i][input_device::button_7] =
            SDL_JoystickGetButton(joysticks_[i], 7) == SDL_PRESSED;
        new_press_key[i][input_device::button_8] =
            SDL_JoystickGetButton(joysticks_[i], 8) == SDL_PRESSED;
        new_press_key[i][input_device::button_9] =
            SDL_JoystickGetButton(joysticks_[i], 9) == SDL_PRESSED;
        new_press_key[i][input_device::start] =
            SDL_JoystickGetButton(joysticks_[i], 10) == SDL_PRESSED;
        new_press_key[i][input_device::select] =
            SDL_JoystickGetButton(joysticks_[i], 11) == SDL_PRESSED;
        new_press_key[i][input_device::up] =
            SDL_JoystickGetAxis(joysticks_[i], 1) < -256;
        new_press_key[i][input_device::down] =
            SDL_JoystickGetAxis(joysticks_[i], 1) > 256;
        new_press_key[i][input_device::left] =
            SDL_JoystickGetAxis(joysticks_[i], 0) < -256;
        new_press_key[i][input_device::right] =
            SDL_JoystickGetAxis(joysticks_[i], 0) > 256;
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    new_press_key[0][input_device::up] =
      state[SDL_SCANCODE_UP] == SDL_PRESSED
      || state[SDL_SCANCODE_K] == SDL_PRESSED;
    new_press_key[0][input_device::down] =
      state[SDL_SCANCODE_DOWN] == SDL_PRESSED
      || state[SDL_SCANCODE_J] == SDL_PRESSED;
    new_press_key[0][input_device::left] =
      state[SDL_SCANCODE_LEFT] == SDL_PRESSED
      || state[SDL_SCANCODE_H] == SDL_PRESSED;
    new_press_key[0][input_device::right] =
      state[SDL_SCANCODE_RIGHT] == SDL_PRESSED
      || state[SDL_SCANCODE_L] == SDL_PRESSED;

    new_press_key[1][input_device::up] = state[SDL_SCANCODE_W] == SDL_PRESSED;
    new_press_key[1][input_device::down] = state[SDL_SCANCODE_S] == SDL_PRESSED;
    new_press_key[1][input_device::left] = state[SDL_SCANCODE_A] == SDL_PRESSED;
    new_press_key[1][input_device::right] = state[SDL_SCANCODE_D] == SDL_PRESSED;

    new_press_key[0][input_device::space] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
    new_press_key[1][input_device::space] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;

    if (debug_mode) {
      new_press_key[0][input_device::b] = state[SDL_SCANCODE_B] == SDL_PRESSED;
      new_press_key[1][input_device::b] = state[SDL_SCANCODE_B] == SDL_PRESSED;
    }

    for (unsigned int i = 0; i < num_devices_; ++i) {
      for (unsigned int j = 0; j < input_device::count; ++j) {
        edge_key_[i][j] = !press_key_[i][j] && new_press_key[i][j];
        press_key_[i][j] = new_press_key[i][j];
      }
    }
  }

  inline bool edge_key_p(const unsigned char player_type,
                         const unsigned char button) const noexcept {
    return edge_key_[player_type][button];
  }

  inline bool press_key_p(const unsigned char player_type,
                          const unsigned char button) const noexcept {
    return press_key_[player_type][button];
  }

  ~InputManager() noexcept {
    for (auto &joystick : joysticks_) {
      SDL_JoystickClose(joystick);
    }
  }
};
