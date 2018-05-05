#include <SDL/SDL_image.h>

#include "def_global.hpp"
#include "input.hpp"

namespace input {

namespace {

const unsigned int Num_devices = 2;

SDL_Joystick *Joystick[Num_devices];
unsigned int Num_joysticks;

}  // namespace

void init_joystick() {
  // SDL_NumJoysticks() definitely returns int type. However, as far as I read
  // source code, it always returns unsigned int type.
  Num_joysticks = static_cast<unsigned int>(SDL_NumJoysticks());
  for (unsigned int i = 0; i < Num_joysticks; ++i) {
    Joystick[i] = SDL_JoystickOpen(i);
  }
}

void update() {
  bool new_press_key[Num_devices][input_device::count];
  for (unsigned int i = 0; i < Num_devices; ++i) {
    for (unsigned int j = 0; j < input_device::count; ++j) {
      new_press_key[i][j] = false;
    }
  }

  for (unsigned int i = 0; i < Num_joysticks; ++i) {
    if (Joystick[i]) {
      SDL_JoystickUpdate();
      new_press_key[i][input_device::x] =
          SDL_JoystickGetButton(Joystick[i], 0) == SDL_PRESSED;
      new_press_key[i][input_device::c] =
          SDL_JoystickGetButton(Joystick[i], 1) == SDL_PRESSED;
      new_press_key[i][input_device::button_2] =
          SDL_JoystickGetButton(Joystick[i], 2) == SDL_PRESSED;
      new_press_key[i][input_device::button_3] =
          SDL_JoystickGetButton(Joystick[i], 3) == SDL_PRESSED;
      new_press_key[i][input_device::button_4] =
          SDL_JoystickGetButton(Joystick[i], 4) == SDL_PRESSED;
      new_press_key[i][input_device::button_5] =
          SDL_JoystickGetButton(Joystick[i], 5) == SDL_PRESSED;
      new_press_key[i][input_device::button_6] =
          SDL_JoystickGetButton(Joystick[i], 6) == SDL_PRESSED;
      new_press_key[i][input_device::button_7] =
          SDL_JoystickGetButton(Joystick[i], 7) == SDL_PRESSED;
      new_press_key[i][input_device::button_8] =
          SDL_JoystickGetButton(Joystick[i], 8) == SDL_PRESSED;
      new_press_key[i][input_device::button_9] =
          SDL_JoystickGetButton(Joystick[i], 9) == SDL_PRESSED;
      new_press_key[i][input_device::start] =
          SDL_JoystickGetButton(Joystick[i], 10) == SDL_PRESSED;
      new_press_key[i][input_device::select] =
          SDL_JoystickGetButton(Joystick[i], 11) == SDL_PRESSED;
      new_press_key[i][input_device::up] =
          SDL_JoystickGetAxis(Joystick[i], 1) < -256;
      new_press_key[i][input_device::down] =
          SDL_JoystickGetAxis(Joystick[i], 1) > 256;
      new_press_key[i][input_device::left] =
          SDL_JoystickGetAxis(Joystick[i], 0) < -256;
      new_press_key[i][input_device::right] =
          SDL_JoystickGetAxis(Joystick[i], 0) > 256;
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
  new_press_key[1][input_device::down] = keys[SDLK_z] == SDL_PRESSED;
  new_press_key[1][input_device::left] = keys[SDLK_a] == SDL_PRESSED;
  new_press_key[1][input_device::right] = keys[SDLK_s] == SDL_PRESSED;

  new_press_key[0][input_device::space] = keys[SDLK_SPACE] == SDL_PRESSED;
  new_press_key[1][input_device::space] = keys[SDLK_SPACE] == SDL_PRESSED;

  if (Debug_mode) {
    new_press_key[0][input_device::b] = keys[SDLK_b] == SDL_PRESSED;
    new_press_key[1][input_device::b] = keys[SDLK_b] == SDL_PRESSED;
  }

  for (unsigned int i = 0; i < Num_devices; ++i) {
    for (unsigned int j = 0; j < input_device::count; ++j) {
      Edge_key[i][j] = !Press_key[i][j] && new_press_key[i][j];
      Press_key[i][j] = new_press_key[i][j];
    }
  }
}

void end_joystick() {
  for (unsigned int i = 0; i < Num_joysticks; ++i) {
    SDL_JoystickClose(Joystick[i]);
  }
}

}  // namespace input
