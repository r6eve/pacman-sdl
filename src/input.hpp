#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include "def_global.hpp"

class Input {
  static const unsigned int num_devices_;
  static unsigned int num_joysticks_;
  static std::vector<SDL_Joystick*> joystick_;

 public:
  static void init_joystick();
  static void update();
  static void end_joystick();
};

#endif
