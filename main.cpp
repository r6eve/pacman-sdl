#define MAIN

#include "main.hpp"
#include <SDL_image.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include "SDL_kanji.hpp"
#include "def_global.hpp"
#include "enemy.hpp"
#include "food.hpp"
#include "image_manager.hpp"
#include "input.hpp"
#include "main_chara.hpp"
#include "map.hpp"
#include "wipe.hpp"

using namespace std;

// #define DEBUG

static Kanji_Font *Font[2];

enum { FONT_SIZE_16, FONT_SIZE_24, NUM_FONT };

static int Blink_count;

int main() {
  if (!init()) {
    return EXIT_FAILURE;
  }
  main_loop();
  end();
  return EXIT_SUCCESS;
}

bool init() {
  try {
    init_sdl();
  } catch (char *e) {
    cerr << "error: " << e << '\n';
    atexit(SDL_Quit);
    return false;
  }

  try {
    init_font();
    init_img();
    init_music();
  } catch (char *e) {
    cerr << "error: " << e << '\n';
    end();
    return false;
  }

  init_color();
  init_joystick();

  Blink_count = 0;
  Enemy_run_debug = false;
  Game_count = 0;
  Game_mode = GAME_MODE_1P;
  Game_state = GAME_STATE_TITLE;
  Num_player = 1;
  Rival_chara_life = 2;

  return true;
}

void init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw SDL_GetError();
  }
  SDL_WM_SetCaption("pacman-sdl", NULL);
  Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  if (!Screen) {
    throw SDL_GetError();
  }

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    throw Mix_GetError();
  }

  SDL_ShowCursor(SDL_DISABLE);
}

void init_font() {
  Font[FONT_SIZE_16] = Kanji_OpenFont("./data/jiskan16.bdf", 16);
  Kanji_AddFont(Font[FONT_SIZE_16], "./data/8x16.bdf");
  Font[FONT_SIZE_24] = Kanji_OpenFont("./data/jiskan24.bdf", 24);
  Kanji_AddFont(Font[FONT_SIZE_24], "./data/12x24.bdf");
  for (int i = 0; i < NUM_FONT; ++i) {
    if (!Font[i]) {
      throw SDL_GetError();
    }
    Kanji_SetCodingSystem(Font[i], KANJI_EUC);
  }
}

void init_img() {
  try {
    load_img("./data/pacman.png", "pacman");
    load_img("./data/rival.png", "rival");
    load_img("./data/bg.png", "bg");
    load_img("./data/bg_red.png", "bg_red");
    load_img("./data/bg_green.png", "bg_green");
    load_img("./data/bg_blue.png", "bg_blue");
    load_img("./data/food.png", "food");
    load_img("./data/food_counter.png", "food_counter");
    load_img("./data/akabei.png", "akabei");
    load_img("./data/pinky.png", "pinky");
    load_img("./data/aosuke.png", "aosuke");
    load_img("./data/guzuta.png", "guzuta");
    load_img("./data/mon_run.png", "mon_run");
    load_img("./data/plate.png", "plate");
  } catch (char *e) {
    throw e;
  }
}

void init_music() {
  Music[0] = Mix_LoadMUS("./data/66376e_Pacman_Siren_Sound_Effect.mp3");
  Music[1] = Mix_LoadMUS("./data/66376e_Pacman_Siren_Sound_Effect.mp3");
  Music[2] = Mix_LoadMUS("./data/pacman_beginning.wav");
  Music[3] = Mix_LoadMUS("./data/pacman_death.wav");
  Se[0] = Mix_LoadWAV("./data/pacman_chomp.wav");
  if (!Music[0] || !Music[1] || !Music[2] || !Music[3] || !Se[0]) {
    throw Mix_GetError();
  }
}

void main_loop() {
  for (;;) {
    update_input();
    switch (Game_state) {
      case GAME_STATE_TITLE:
        title();
        break;
      case GAME_STATE_START:
        game_start();
        break;
      case GAME_STATE_GAME:
        play_game();
        break;
      case GAME_STATE_CLEAR:
        game_clear();
        break;
      case GAME_STATE_MISS:
        game_miss();
        break;
      case GAME_STATE_OVER:
        game_over();
        break;
      case GAME_STATE_PAUSE:
        game_pause();
        break;
      default:
        cerr << "error: undefined state." << '\n';
        return;
    }
    if (!poll_event()) {
      return;
    }
#ifdef DEBUG
    draw_fps();
#endif
    SDL_Flip(SDL_GetVideoSurface());
    wait_game();
  }
}

void title() {
  SDL_Rect dst_back, dst_select;
  Uint32 white = 0xffffffff;
  Uint32 black = 0x00000000;
  dst_back.x = 0;
  dst_back.y = 0;
  dst_back.w = SCREEN_WIDTH;
  dst_back.h = SCREEN_HEIGHT;
  SDL_FillRect(Screen, &dst_back, white);

  switch (Game_count) {
    case 0:
      set_wipe_in();
      draw_wipe(SCREEN_WIDTH);
      ++Game_count;
      break;
    case 1:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");
      draw_wipe(SCREEN_WIDTH);
      if (update_wipe()) {
        ++Game_count;
      }
      break;
    case 2:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");
      if (Blink_count < 30) {
        Kanji_PutText(Screen, 205, 300, Font[FONT_SIZE_16], BLACK,
                      "P r e s s  S p a c e  K e y");
        ++Blink_count;
      } else if (Blink_count < 60) {
        ++Blink_count;
      } else {
        Blink_count = 0;
      }

      if (Edge_key[0][PRESS_KEY_X] || Edge_key[1][PRESS_KEY_X] ||
          Edge_key[0][PRESS_KEY_SPACE]) {
        ++Game_count;
        Blink_count = 0;
      }
      break;
    case 3:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");
      if (!Press_key[0][PRESS_KEY_X] && !Press_key[1][PRESS_KEY_X] &&
          !Press_key[0][PRESS_KEY_SPACE]) {
        ++Game_count;
      }
      break;
    case 4:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");

      switch (Game_mode) {
        case GAME_MODE_1P:
          dst_select.x = 250;
          dst_select.y = 300;
          dst_select.w = 96;
          dst_select.h = 16;
          SDL_FillRect(Screen, &dst_select, black);
          Kanji_PutText(Screen, 270, 300, Font[FONT_SIZE_16], WHITE, "1P MODE");
          Kanji_PutText(Screen, 270, 350, Font[FONT_SIZE_16], BLACK, "VS MODE");
          break;
        case GAME_MODE_BATTLE:
          dst_select.x = 250;
          dst_select.y = 350;
          dst_select.w = 96;
          dst_select.h = 16;
          SDL_FillRect(Screen, &dst_select, black);
          Kanji_PutText(Screen, 270, 300, Font[FONT_SIZE_16], BLACK, "1P MODE");
          Kanji_PutText(Screen, 270, 350, Font[FONT_SIZE_16], WHITE, "VS MODE");
          break;
        default:
          cerr << "error: undefined game mode." << '\n';
          break;
      }

      if (Press_key[0][PRESS_KEY_X] || Press_key[1][PRESS_KEY_X] ||
          Press_key[0][PRESS_KEY_SPACE]) {
        set_wipe_out();
        draw_wipe(SCREEN_WIDTH);
        ++Game_count;
      }

      if (Press_key[0][PRESS_KEY_BUTTON_2] ||
          Press_key[1][PRESS_KEY_BUTTON_2]) {
        Game_count -= 2;
        Game_mode = GAME_MODE_1P;
      }

      if (Press_key[0][PRESS_KEY_DOWN] || Press_key[1][PRESS_KEY_DOWN]) {
        Game_mode = GAME_MODE_BATTLE;
      } else if (Press_key[0][PRESS_KEY_UP] || Press_key[1][PRESS_KEY_UP]) {
        Game_mode = GAME_MODE_1P;
      }
      break;
    case 5:
      switch (Game_mode) {
        case GAME_MODE_1P:
          dst_select.x = 250;
          dst_select.y = 300;
          dst_select.w = 96;
          dst_select.h = 16;
          SDL_FillRect(Screen, &dst_select, black);
          Kanji_PutText(Screen, 270, 300, Font[FONT_SIZE_16], WHITE, "1P MODE");
          Kanji_PutText(Screen, 270, 350, Font[FONT_SIZE_16], BLACK, "VS MODE");
          break;
        case GAME_MODE_BATTLE:
          dst_select.x = 250;
          dst_select.y = 350;
          dst_select.w = 96;
          dst_select.h = 16;
          SDL_FillRect(Screen, &dst_select, black);
          Kanji_PutText(Screen, 270, 300, Font[FONT_SIZE_16], BLACK, "1P MODE");
          Kanji_PutText(Screen, 270, 350, Font[FONT_SIZE_16], WHITE, "VS MODE");
          break;
        default:
          cerr << "error: undefined game mode." << "\n";
          break;
      }

      draw_wipe(SCREEN_WIDTH);

      if (update_wipe()) {
        init_map();
        init_food();
        init_main_chara();
        init_enemy();

        Game_count = 0;
        Game_state = GAME_STATE_START;
        Game_level = 1;
        Main_chara_life = 2;

        if (Game_mode == GAME_MODE_BATTLE) {
          Num_player = 2;
        }

        for (int i = 0; i < Num_player; ++i) {
          Now_score[i] = 0;
        }

        srand((unsigned int)time(NULL));
      }
      break;
    default:
      cerr << "error: undefined game count." << '\n';
      break;
  }
}

void game_start() {
  draw_map();
  draw_food();
  draw_enemy();
  draw_main_chara();
  draw_score();
  switch (Game_count) {
    case 0:
      if ((Main_chara_life == 2) && (Rival_chara_life == 2)) {
        Mix_PlayMusic(Music[2], 0);
      }
      set_wipe_in();
      draw_wipe(OFFSET_X);
      ++Game_count;
      break;
    case 1:
      draw_wipe(OFFSET_X);
      if (update_wipe()) {
        ++Game_count;
      }
      break;
    default:
      ++Game_count;
      break;
  }
  if (Game_count < 130) {
    Kanji_PutText(Screen, 220, 200, Font[FONT_SIZE_24], RED, "S t a g e %d",
                  Game_level);
  } else if (Game_count < 200) {
    Kanji_PutText(Screen, 220, 200, Font[FONT_SIZE_24], RED, "S t a r t");
  }

  if (Game_count > 220) {
    Game_count = 0;
    Game_state = GAME_STATE_GAME;
    for (int i = 0; i < Num_player; ++i) {
      Power_chara_mode[i] = 0;
    }
    for (int i = 0; i < NUM_ENEMY; ++i) {
      Enemy_state[i] = ENEMY_STATE_NORMAL;
    }
  }
}

void play_game() {
  update_food();
  draw_map();
  draw_food();
  draw_enemy();
  draw_main_chara();
  draw_score();
  mv_main_chara();
  for (int i = 0; i < NUM_ENEMY; ++i) {
    if (Enemy_run_debug || (Enemy_state[i] == ENEMY_STATE_LOSE)) {
      mv_lose_enemy(i);
    } else {
      mv_normal_enemy(i);
    }
  }
  check_food_state();
  check_hit_enemy();

  if (Edge_key[0][PRESS_KEY_SPACE]) {
    Game_state = GAME_STATE_PAUSE;
  }

  if (Edge_key[0][PRESS_KEY_B]) {
    Enemy_run_debug = !Enemy_run_debug;
  }
}

void game_clear() {
  draw_map();
  draw_food();
  draw_enemy();
  draw_main_chara();
  draw_score();

  if (Game_count == 0) {
    set_wipe_out();
    draw_wipe(OFFSET_X);
    ++Game_count;
  } else if (Game_count == 1) {
    draw_wipe(OFFSET_X);
    if (update_wipe()) {
      if (Game_level >= 256) {
        Game_count = 0;
        Game_state = GAME_STATE_OVER;
      } else {
        Game_count = 0;
        Game_state = GAME_STATE_START;
        ++Game_level;
        init_food();
        init_enemy();
        init_main_chara();
      }
    }
  }
}

void game_miss() {
  draw_map();
  draw_food();
  draw_enemy();
  draw_main_chara();
  draw_score();

  if (Game_count == 0) {
    Mix_PlayMusic(Music[3], 0);
    set_wipe_out();
    if ((Main_chara_life == 0) || (Rival_chara_life == 0)) {
      draw_wipe(SCREEN_WIDTH);
    } else {
      draw_wipe(OFFSET_X);
    }
    ++Game_count;
  } else if (Game_count == 1) {
    if ((Main_chara_life == 0) || (Rival_chara_life == 0)) {
      draw_wipe(SCREEN_WIDTH);
    } else {
      draw_wipe(OFFSET_X);
    }

    if (Choice_hit) {
      add_main_chara_pos(0, -1);
      if (update_wipe()) {
        --Main_chara_life;
        if (Main_chara_life >= 0) {
          Game_count = 0;
          Game_state = GAME_STATE_START;
          init_enemy();
          init_main_chara();
        } else {
          Game_count = 0;
          Blink_count = 0;
          Game_state = GAME_STATE_OVER;
        }
      }
    } else {
      add_rival_chara_pos(0, -1);
      if (update_wipe()) {
        --Rival_chara_life;
        if (Rival_chara_life >= 0) {
          Game_count = 0;
          Game_state = GAME_STATE_START;
          init_enemy();
          init_main_chara();
        } else {
          Game_count = 0;
          Blink_count = 0;
          Game_state = GAME_STATE_OVER;
        }
      }
    }
  }
}

void game_over() {
  SDL_Rect dst_back = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  Uint32 white = 0xffffffff;
  SDL_FillRect(Screen, &dst_back, white);

  switch (Game_mode) {
    case GAME_MODE_1P:
      switch (Game_count) {
        case 0:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          set_wipe_in();
          draw_wipe(SCREEN_WIDTH);
          ++Game_count;
          break;
        case 1:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          draw_wipe(SCREEN_WIDTH);
          if (update_wipe()) {
            ++Game_count;
          }
          break;
        case 2:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          Kanji_PutText(Screen, 150, 225, Font[FONT_SIZE_24], BLACK,
                        "Y o u r  S c o r e :  %d", Now_score[0]);

          if (Blink_count < 30) {
            Kanji_PutText(Screen, 205, 300, Font[FONT_SIZE_16], BLACK,
                          "P r e s s  S p a c e  K e y");
            ++Blink_count;
          } else if (Blink_count < 60) {
            ++Blink_count;
          } else {
            Blink_count = 0;
          }

          if (Press_key[0][PRESS_KEY_X] || Press_key[1][PRESS_KEY_X] ||
              Press_key[0][PRESS_KEY_SPACE]) {
            ++Game_count;
            set_wipe_out();
            draw_wipe(SCREEN_WIDTH);
          }
          break;
        case 3:
          draw_wipe(SCREEN_WIDTH);
          if (update_wipe()) {
            Blink_count = 0;
            Game_count = 0;
            Game_state = GAME_STATE_TITLE;
          }
          break;
        default:
          cerr << "error: undefined game count." << '\n';
          break;
      }
      break;
    case GAME_MODE_BATTLE:
      switch (Game_count) {
        case 0:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          set_wipe_in();
          draw_wipe(SCREEN_WIDTH);
          ++Game_count;
          break;
        case 1:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          draw_wipe(SCREEN_WIDTH);
          if (update_wipe()) {
            ++Game_count;
          }
          break;
        case 2:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          if (Now_score[0] > Now_score[1]) {
            Kanji_PutText(Screen, 210, 300, Font[FONT_SIZE_24], BLACK,
                          "1 P  W I N :  %d", Now_score[0]);
          } else if (Now_score[1] > Now_score[0]) {
            Kanji_PutText(Screen, 210, 300, Font[FONT_SIZE_24], BLACK,
                          "2 P  W I N :  %d", Now_score[1]);
          } else {
            Kanji_PutText(Screen, 210, 300, Font[FONT_SIZE_24], BLACK,
                          "D R A W :  %d", Now_score[0]);
          }

          if (Blink_count < 30) {
            Kanji_PutText(Screen, 240, 400, Font[FONT_SIZE_16], BLACK,
                          "P r e s s  K e y");
            ++Blink_count;
          } else if (Blink_count < 60) {
            ++Blink_count;
          } else {
            Blink_count = 0;
          }

          if (Press_key[0][PRESS_KEY_X] || Press_key[1][PRESS_KEY_X] ||
              Press_key[0][PRESS_KEY_SPACE]) {
            ++Game_count;
            set_wipe_out();
            draw_wipe(SCREEN_WIDTH);
          }
          break;
        case 3:
          draw_wipe(SCREEN_WIDTH);
          if (update_wipe()) {
            Blink_count = 0;
            Game_count = 0;
            Rival_chara_life = 2;
            Game_state = GAME_STATE_TITLE;
          }
          break;
        default:
          cerr << "error: undefined game count." << '\n';
          break;
      }
      break;
    default:
      cerr << "error: undefined game state." << '\n';
      break;
  }
}

void game_pause() {
  draw_map();
  draw_food();
  draw_enemy();
  draw_main_chara();
  draw_score();
  draw_translucence();
  if (Edge_key[0][PRESS_KEY_SPACE]) {
    Game_state = GAME_STATE_GAME;
  }
}

void draw_score() {
  {
    SDL_Surface *p_surface = get_img("plate");
    SDL_Rect dst;
    dst.x = OFFSET_X;
    dst.y = 0;
    SDL_BlitSurface(p_surface, NULL, Screen, &dst);
  }
  {
    Kanji_PutText(Screen, OFFSET_X + 10, SCREEN_HEIGHT / 6, Font[FONT_SIZE_16],
                  WHITE, "Score: %6d", Now_score[0]);
    SDL_Surface *p_surface = get_img("pacman");
    SDL_Rect src, dst;
    src.x = BLOCK_SIZE;
    src.y = 0;
    src.w = BLOCK_SIZE;
    src.h = BLOCK_SIZE;
    dst.x = OFFSET_X + 60;
    dst.y = (SCREEN_HEIGHT / 6 + 32) - 5;
    SDL_BlitSurface(p_surface, &src, Screen, &dst);
    Kanji_PutText(Screen, OFFSET_X + 80, SCREEN_HEIGHT / 6 + 32,
                  Font[FONT_SIZE_16], WHITE, " x %d", Main_chara_life);
    if (Game_mode == GAME_MODE_BATTLE) {
      Kanji_PutText(Screen, OFFSET_X + 10, SCREEN_HEIGHT / 6 + 80,
                    Font[FONT_SIZE_16], WHITE, "Score: %6d", Now_score[1]);
      SDL_Surface *p_surface = get_img("rival");
      SDL_Rect src, dst;
      src.x = BLOCK_SIZE;
      src.y = 0;
      src.w = BLOCK_SIZE;
      src.h = BLOCK_SIZE;
      dst.x = OFFSET_X + 60;
      dst.y = (SCREEN_HEIGHT / 6 + 112) - 5;
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      Kanji_PutText(Screen, OFFSET_X + 80, SCREEN_HEIGHT / 6 + 112,
                    Font[FONT_SIZE_16], WHITE, " x %d", Rival_chara_life);
    }
  }
  {
    if (Power_chara_mode[0]) {
      Uint32 yellow = 0xffff00;
      SDL_Rect dst;

      dst.x = OFFSET_X + 10;
      dst.y = SCREEN_HEIGHT / 6 * 4;
      dst.w = Power_chara_mode[0] / 4;
      dst.h = BLOCK_SIZE;
      SDL_FillRect(Screen, &dst, yellow);
      --Power_chara_mode[0];
    }
    if (Power_chara_mode[1]) {
      Uint32 gray = 0x808080;
      SDL_Rect dst;

      dst.x = OFFSET_X + 10;
      dst.y = SCREEN_HEIGHT / 6 * 4 + 30;
      dst.w = Power_chara_mode[1] / 4;
      dst.h = BLOCK_SIZE;
      SDL_FillRect(Screen, &dst, gray);
      --Power_chara_mode[1];
    }
  }
}

int poll_event() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return 0;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return 0;
        }
        break;
      default:
        // do nothing
        break;
    }
  }
  return 1;
}

void wait_game() {
  static Uint32 pre_count;
  double wait_time = 1000.0 / FPS_MAX;
  Uint32 wait_count = (wait_time + 0.5);
  if (pre_count) {
    Uint32 now_count = SDL_GetTicks();
    Uint32 interval = now_count - pre_count;
    if (interval < wait_count) {
      Uint32 delay_time = wait_count - interval;
      SDL_Delay(delay_time);
    }
  }
  pre_count = SDL_GetTicks();
}

void draw_fps() {
  static Uint32 pre_count;
  Uint32 now_count = SDL_GetTicks();
  if (pre_count) {
    static double frame_rate;
    Uint32 interval = now_count - pre_count;
    if (interval < 1) {
      interval = 1;
    }

    if (!(pre_count % 30)) {
      frame_rate = 1000.0 / interval;
    }

    Kanji_PutText(Screen, OFFSET_X + 20, 16, Font[FONT_SIZE_16], WHITE,
                  "FrameRate[%0.2f]", frame_rate);
  }
  pre_count = now_count;
}

void end() {
  del_all_img();
  for (int i = 0; i < NUM_FONT; ++i) {
    Kanji_CloseFont(Font[i]);
  }
  end_joystick();
  end_music();
  atexit(SDL_Quit);
}

void end_music() {
  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  // TODO: What's the number of 2?
  for (int i = 0; i < 2; ++i) {
    Mix_FreeMusic(Music[i]);
  }
  Mix_FreeChunk(Se[0]);
  Mix_CloseAudio();
}

int get_distance(int x1, int y1, int x2, int y2) {
  return (int)hypot((double)(x1 - x2), (double)(y1 - y2));
}

void draw_translucence() {
  Uint32 rmask, gmask, bmask;
  Uint8 alpha = 128;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
#endif
  SDL_Rect dst_back;
  dst_back.x = 0;
  dst_back.y = 0;

  SDL_Surface *trans_surface = SDL_CreateRGBSurface(
      SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, 0);
  if (trans_surface == NULL) {
    cerr << "CreateRGBSurface failed: " <<  SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(trans_surface, NULL, Screen, &dst_back);
  if (Blink_count < 30) {
    Kanji_PutText(Screen, 240, 200, Font[FONT_SIZE_24], WHITE, "P a u s e");
    ++Blink_count;
  } else if (Blink_count < 60) {
    ++Blink_count;
  } else {
    Blink_count = 0;
  }
}
