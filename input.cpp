#include "input.hpp"
#include <SDL_image.h>
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
  int new_press_key[2][NUM_PRESS_KEY];  // 1: press, 0: not
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < NUM_PRESS_KEY; ++j) {
      new_press_key[i][j] = 0;
    }
  }

  for (int i = 0; i < Num_joysticks; ++i) {
    if (Joystick[i]) {
      SDL_JoystickUpdate();
      if (SDL_JoystickGetButton(Joystick[i], 0) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_0] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 1) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_1] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 2) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_2] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 3) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_3] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 4) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_4] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 5) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_5] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 6) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_6] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 7) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_7] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 8) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_8] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 9) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_9] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 10) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_10] = 1;
      }
      if (SDL_JoystickGetButton(Joystick[i], 11) == SDL_PRESSED) {
        new_press_key[i][PRESS_KEY_BUTTON_11] = 1;
      }
      if (SDL_JoystickGetAxis(Joystick[i], 1) < -256) {
        new_press_key[i][PRESS_KEY_UP] = 1;
      }
      if (SDL_JoystickGetAxis(Joystick[i], 1) > 256) {
        new_press_key[i][PRESS_KEY_DOWN] = 1;
      }
      if (SDL_JoystickGetAxis(Joystick[i], 0) < -256) {
        new_press_key[i][PRESS_KEY_LEFT] = 1;
      }
      if (SDL_JoystickGetAxis(Joystick[i], 0) > 256) {
        new_press_key[i][PRESS_KEY_RIGHT] = 1;
      }
    }
  }

  Uint8 *keys = SDL_GetKeyState(NULL);
  if ((keys[SDLK_UP] == SDL_PRESSED) || (keys[(unsigned int)'k'] == SDL_PRESSED)) {
    new_press_key[0][PRESS_KEY_UP] = 1;
  }
  if ((keys[SDLK_DOWN] == SDL_PRESSED) || (keys[(unsigned int)'j'] == SDL_PRESSED)) {
    new_press_key[0][PRESS_KEY_DOWN] = 1;
  }
  if ((keys[SDLK_LEFT] == SDL_PRESSED) || (keys[(unsigned int)'h'] == SDL_PRESSED)) {
    new_press_key[0][PRESS_KEY_LEFT] = 1;
  }
  if ((keys[SDLK_RIGHT] == SDL_PRESSED) || (keys[(unsigned int)'l'] == SDL_PRESSED)) {
    new_press_key[0][PRESS_KEY_RIGHT] = 1;
  }
  if (keys[(unsigned int)'c'] == SDL_PRESSED) {
    new_press_key[0][PRESS_KEY_BUTTON_2] = 1;
  }
#ifdef DEBUG
  if (keys[(unsigned int)'b'] == SDL_PRESSED) {
    new_press_key[0][PRESS_KEY_BUTTON_B] = 1;
  }
#endif
  if (keys[SDLK_SPACE] == SDL_PRESSED) {
    new_press_key[0][PRESS_KEY_SPACE] = 1;
  }
  if (keys[(unsigned int)'w'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_UP] = 1;
  }
  if (keys[(unsigned int)'z'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_DOWN] = 1;
  }
  if (keys[(unsigned int)'a'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_LEFT] = 1;
  }
  if (keys[(unsigned int)'s'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_RIGHT] = 1;
  }
  if (keys[(unsigned int)'x'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_BUTTON_0] = 1;
  }
  if (keys[(unsigned int)'c'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_BUTTON_1] = 1;
  }
#ifdef DEBUG
  if (keys[(unsigned int)'b'] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_BUTTON_B] = 1;
  }
#endif
  if (keys[SDLK_SPACE] == SDL_PRESSED) {
    new_press_key[1][PRESS_KEY_SPACE] = 1;
  }
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < NUM_PRESS_KEY; ++j) {
      Edge_key[i][j] = 0;
      if (!Press_key[i][j] && new_press_key[i][j]) {
        Edge_key[i][j] = 1;
      }
      Press_key[i][j] = new_press_key[i][j];
    }
  }
}

void end_joystick() {
  for (int i = 0; i < Num_joysticks; ++i) {
    SDL_JoystickClose(Joystick[i]);
  }
}
