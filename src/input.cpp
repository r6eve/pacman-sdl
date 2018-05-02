#include "input.hpp"
#include <SDL/SDL_image.h>
#include "def_global.hpp"

namespace input {

static SDL_Joystick *Joystick[2];  // TODO: 2
static int Num_joysticks;

void init_joystick() {
  Num_joysticks = SDL_NumJoysticks();
  for (int i = 0; i < Num_joysticks; ++i) {
    Joystick[i] = SDL_JoystickOpen(i);
  }
}

void update_input() {
  bool new_press_key[NUM_DEVICES][input_device::count];
  for (int i = 0; i < NUM_DEVICES; ++i) {
    for (int j = 0; j < input_device::count; ++j) {
      new_press_key[i][j] = false;
    }
  }

  for (int i = 0; i < Num_joysticks; ++i) {
    if (Joystick[i]) {
      SDL_JoystickUpdate();
      new_press_key[i][input_device::x] =
          SDL_JoystickGetButton(Joystick[i], 0) == SDL_PRESSED;
      new_press_key[i][input_device::c] =
          SDL_JoystickGetButton(Joystick[i], 1) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_2] =
          SDL_JoystickGetButton(Joystick[i], 2) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_3] =
          SDL_JoystickGetButton(Joystick[i], 3) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_4] =
          SDL_JoystickGetButton(Joystick[i], 4) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_5] =
          SDL_JoystickGetButton(Joystick[i], 5) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_6] =
          SDL_JoystickGetButton(Joystick[i], 6) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_7] =
          SDL_JoystickGetButton(Joystick[i], 7) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_8] =
          SDL_JoystickGetButton(Joystick[i], 8) == SDL_PRESSED;
      new_press_key[i][input_device::bUTTON_9] =
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
#ifdef DEBUG
  new_press_key[0][input_device::b] = keys[SDLK_b] == SDL_PRESSED;
  new_press_key[1][input_device::b] = keys[SDLK_b] == SDL_PRESSED;
#endif
  for (int i = 0; i < NUM_DEVICES; ++i) {
    for (int j = 0; j < input_device::count; ++j) {
      Edge_key[i][j] = !Press_key[i][j] && new_press_key[i][j];
      Press_key[i][j] = new_press_key[i][j];
    }
  }
}

void end_joystick() {
  for (int i = 0; i < Num_joysticks; ++i) {
    SDL_JoystickClose(Joystick[i]);
  }
}

}  // namespace input
