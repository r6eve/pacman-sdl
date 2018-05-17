#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <vector>
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
  unsigned int num_joysticks_;
  std::vector<SDL_Joystick*> joystick_;
  bool edge_key_[2][input_device::count];
  bool press_key_[2][input_device::count];

 public:
  InputManager() noexcept {
    // SDL_NumJoysticks() definitely returns int type. However, as far as I read
    // source code, it always returns unsigned int type.
    num_joysticks_ = static_cast<unsigned int>(SDL_NumJoysticks());
    for (unsigned int i = 0; i < num_joysticks_; ++i) {
      joystick_[i] = SDL_JoystickOpen(i);
    }
  }

  void update(const bool debug_mode) noexcept;

  inline bool edge_key_p(const unsigned char player_type,
                         const unsigned char button) const noexcept {
    return edge_key_[player_type][button];
  }

  inline bool press_key_p(const unsigned char player_type,
                          const unsigned char button) const noexcept {
    return press_key_[player_type][button];
  }

  ~InputManager() noexcept {
    for (unsigned int i = 0; i < num_joysticks_; ++i) {
      SDL_JoystickClose(joystick_[i]);
    }
  }
};

#endif
