#include "input_manager.hpp"
#include <vector>
#include "def_global.hpp"

InputManager::InputManager() noexcept {
  // SDL_NumJoysticks() definitely returns int type. However, as far as I read
  // source code, it always returns unsigned int type.
  num_joysticks_ = static_cast<unsigned int>(SDL_NumJoysticks());
  for (unsigned int i = 0; i < num_joysticks_; ++i) {
    joystick_[i] = SDL_JoystickOpen(i);
  }
}

void InputManager::update(const bool debug_mode) noexcept {
  constexpr unsigned int num_devices_ = 2;
  bool new_press_key[num_devices_][input_device::count];
  for (unsigned int i = 0; i < num_devices_; ++i) {
    for (unsigned int j = 0; j < input_device::count; ++j) {
      new_press_key[i][j] = false;
    }
  }

  for (unsigned int i = 0; i < num_joysticks_; ++i) {
    if (joystick_[i]) {
      SDL_JoystickUpdate();
      new_press_key[i][input_device::x] =
          SDL_JoystickGetButton(joystick_[i], 0) == SDL_PRESSED;
      new_press_key[i][input_device::c] =
          SDL_JoystickGetButton(joystick_[i], 1) == SDL_PRESSED;
      new_press_key[i][input_device::button_2] =
          SDL_JoystickGetButton(joystick_[i], 2) == SDL_PRESSED;
      new_press_key[i][input_device::button_3] =
          SDL_JoystickGetButton(joystick_[i], 3) == SDL_PRESSED;
      new_press_key[i][input_device::button_4] =
          SDL_JoystickGetButton(joystick_[i], 4) == SDL_PRESSED;
      new_press_key[i][input_device::button_5] =
          SDL_JoystickGetButton(joystick_[i], 5) == SDL_PRESSED;
      new_press_key[i][input_device::button_6] =
          SDL_JoystickGetButton(joystick_[i], 6) == SDL_PRESSED;
      new_press_key[i][input_device::button_7] =
          SDL_JoystickGetButton(joystick_[i], 7) == SDL_PRESSED;
      new_press_key[i][input_device::button_8] =
          SDL_JoystickGetButton(joystick_[i], 8) == SDL_PRESSED;
      new_press_key[i][input_device::button_9] =
          SDL_JoystickGetButton(joystick_[i], 9) == SDL_PRESSED;
      new_press_key[i][input_device::start] =
          SDL_JoystickGetButton(joystick_[i], 10) == SDL_PRESSED;
      new_press_key[i][input_device::select] =
          SDL_JoystickGetButton(joystick_[i], 11) == SDL_PRESSED;
      new_press_key[i][input_device::up] =
          SDL_JoystickGetAxis(joystick_[i], 1) < -256;
      new_press_key[i][input_device::down] =
          SDL_JoystickGetAxis(joystick_[i], 1) > 256;
      new_press_key[i][input_device::left] =
          SDL_JoystickGetAxis(joystick_[i], 0) < -256;
      new_press_key[i][input_device::right] =
          SDL_JoystickGetAxis(joystick_[i], 0) > 256;
    }
  }

  // TODO: enum class 0: player 1, 1: player 2
  Uint8 *keys = SDL_GetKeyState(nullptr);
  new_press_key[0][input_device::up] =
      (keys[SDLK_UP] == SDL_PRESSED) || (keys[SDLK_k] == SDL_PRESSED);
  new_press_key[0][input_device::down] =
      (keys[SDLK_DOWN] == SDL_PRESSED) || (keys[SDLK_j] == SDL_PRESSED);
  new_press_key[0][input_device::left] =
      (keys[SDLK_LEFT] == SDL_PRESSED) || (keys[SDLK_h] == SDL_PRESSED);
  new_press_key[0][input_device::right] =
      (keys[SDLK_RIGHT] == SDL_PRESSED) || (keys[SDLK_l] == SDL_PRESSED);
  new_press_key[1][input_device::up] = keys[SDLK_w] == SDL_PRESSED;
  new_press_key[1][input_device::down] = keys[SDLK_s] == SDL_PRESSED;
  new_press_key[1][input_device::left] = keys[SDLK_a] == SDL_PRESSED;
  new_press_key[1][input_device::right] = keys[SDLK_d] == SDL_PRESSED;

  new_press_key[0][input_device::space] = keys[SDLK_SPACE] == SDL_PRESSED;
  new_press_key[1][input_device::space] = keys[SDLK_SPACE] == SDL_PRESSED;

  if (debug_mode) {
    new_press_key[0][input_device::b] = keys[SDLK_b] == SDL_PRESSED;
    new_press_key[1][input_device::b] = keys[SDLK_b] == SDL_PRESSED;
  }

  for (unsigned int i = 0; i < num_devices_; ++i) {
    for (unsigned int j = 0; j < input_device::count; ++j) {
      edge_key_[i][j] = !press_key_[i][j] && new_press_key[i][j];
      press_key_[i][j] = new_press_key[i][j];
    }
  }
}

bool InputManager::edge_key_p(const unsigned int player_type,
                              const unsigned int button) const noexcept {
  return edge_key_[player_type][button];
}

bool InputManager::press_key_p(const unsigned int player_type,
                               const unsigned int button) const noexcept {
  return press_key_[player_type][button];
}

InputManager::~InputManager() noexcept {
  for (unsigned int i = 0; i < num_joysticks_; ++i) {
    SDL_JoystickClose(joystick_[i]);
  }
}
