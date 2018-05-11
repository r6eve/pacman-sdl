#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <vector>
#include "def_global.hpp"

class Input_manager {
  static const unsigned int num_devices_;
  static unsigned int num_joysticks_;
  static std::vector<SDL_Joystick*> joystick_;

 public:
  static void init_joystick() noexcept;
  static void update(const bool debug_mode) noexcept;
  static void end_joystick() noexcept;
};

#endif
