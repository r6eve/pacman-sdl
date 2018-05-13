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

class Input_manager {
  static const unsigned int num_devices_;
  static unsigned int num_joysticks_;
  static std::vector<SDL_Joystick*> joystick_;
  static bool edge_key_[2][input_device::count];
  static bool press_key_[2][input_device::count];

 public:
  static void init_joystick() noexcept;
  static void update(const bool debug_mode) noexcept;
  static void end_joystick() noexcept;
  static bool edge_key_p(const unsigned int user,
                         const unsigned int button) noexcept;
  static bool press_key_p(const unsigned int user,
                          const unsigned int button) noexcept;
};

#endif
