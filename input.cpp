#include "input.hpp"
#include <SDL/SDL_image.h>
#include "def_global.hpp"

#define DEBUG

static SDL_Joystick *Joystick[2];
static int Num_joysticks;

void init_joystick() {
  Num_joysticks = SDL_NumJoysticks();
  for (int i = 0; i < Num_joysticks; ++i) {
    Joystick[i] = SDL_JoystickOpen(i);
  }
}

void update_input() {
  bool new_press_key[NUM_DEVICES][NUM_PRESS_KEY];
  for (int i = 0; i < NUM_DEVICES; ++i) {
    for (int j = 0; j < NUM_PRESS_KEY; ++j) {
      new_press_key[i][j] = false;
    }
  }

  for (int i = 0; i < Num_joysticks; ++i) {
    if (Joystick[i]) {
      SDL_JoystickUpdate();
      new_press_key[i][PRESS_KEY_X] =
          SDL_JoystickGetButton(Joystick[i], 0) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_C] =
          SDL_JoystickGetButton(Joystick[i], 1) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_2] =
          SDL_JoystickGetButton(Joystick[i], 2) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_3] =
          SDL_JoystickGetButton(Joystick[i], 3) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_4] =
          SDL_JoystickGetButton(Joystick[i], 4) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_5] =
          SDL_JoystickGetButton(Joystick[i], 5) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_6] =
          SDL_JoystickGetButton(Joystick[i], 6) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_7] =
          SDL_JoystickGetButton(Joystick[i], 7) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_8] =
          SDL_JoystickGetButton(Joystick[i], 8) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_BUTTON_9] =
          SDL_JoystickGetButton(Joystick[i], 9) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_START] =
          SDL_JoystickGetButton(Joystick[i], 10) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_SELECT] =
          SDL_JoystickGetButton(Joystick[i], 11) == SDL_PRESSED;
      new_press_key[i][PRESS_KEY_UP] =
          SDL_JoystickGetAxis(Joystick[i], 1) < -256;
      new_press_key[i][PRESS_KEY_DOWN] =
          SDL_JoystickGetAxis(Joystick[i], 1) > 256;
      new_press_key[i][PRESS_KEY_LEFT] =
          SDL_JoystickGetAxis(Joystick[i], 0) < -256;
      new_press_key[i][PRESS_KEY_RIGHT] =
          SDL_JoystickGetAxis(Joystick[i], 0) > 256;
    }
  }

  Uint8 *keys = SDL_GetKeyState(NULL);
  new_press_key[0][PRESS_KEY_UP] =
      (keys[SDLK_UP] == SDL_PRESSED) || (keys[SDLK_k] == SDL_PRESSED);
  new_press_key[0][PRESS_KEY_DOWN] =
      (keys[SDLK_DOWN] == SDL_PRESSED) || (keys[SDLK_j] == SDL_PRESSED);
  new_press_key[0][PRESS_KEY_LEFT] =
      (keys[SDLK_LEFT] == SDL_PRESSED) || (keys[SDLK_h] == SDL_PRESSED);
  new_press_key[0][PRESS_KEY_RIGHT] =
      (keys[SDLK_RIGHT] == SDL_PRESSED) || (keys[SDLK_l] == SDL_PRESSED);
  new_press_key[1][PRESS_KEY_UP] = keys[SDLK_w] == SDL_PRESSED;
  new_press_key[1][PRESS_KEY_DOWN] = keys[SDLK_z] == SDL_PRESSED;
  new_press_key[1][PRESS_KEY_LEFT] = keys[SDLK_a] == SDL_PRESSED;
  new_press_key[1][PRESS_KEY_RIGHT] = keys[SDLK_s] == SDL_PRESSED;

  new_press_key[0][PRESS_KEY_SPACE] = keys[SDLK_SPACE] == SDL_PRESSED;
  new_press_key[1][PRESS_KEY_SPACE] = keys[SDLK_SPACE] == SDL_PRESSED;
#ifdef DEBUG
  new_press_key[0][PRESS_KEY_B] = keys[SDLK_b] == SDL_PRESSED;
  new_press_key[1][PRESS_KEY_B] = keys[SDLK_b] == SDL_PRESSED;
#endif
  for (int i = 0; i < NUM_DEVICES; ++i) {
    for (int j = 0; j < NUM_PRESS_KEY; ++j) {
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
