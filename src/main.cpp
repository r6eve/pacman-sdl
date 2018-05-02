#define MAIN

#include "main.hpp"
#include <SDL/SDL_image.h>
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

static Kanji_Font *Font[2];

// TODO: enum class
enum { FONT_SIZE_16, FONT_SIZE_24, NUM_FONT };

static int Blink_count;

int main(int, char **) {
  if (!init()) {
    return EXIT_FAILURE;
  }
  main_loop();
  end();
  return EXIT_SUCCESS;
}

bool init() {
  // TODO: exit() here
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
  input::init_joystick();

  // initialize global variables
  Blink_count = 0;
  Enemy_run_debug = false;
  Game_count = 0;
  Game_mode = game_mode::single;
  Game_state = game_state::title;
  Num_player = 1;
  Rival_chara_life = 2;

  return true;
}

void init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw SDL_GetError();
  }
  SDL_WM_SetCaption("pacman-sdl", nullptr);
#ifdef DEBUG
  Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                            SDL_HWSURFACE | SDL_DOUBLEBUF);
#else
  Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
#endif
  if (!Screen) {
    throw SDL_GetError();
  }

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
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
    image_manager::load_img("./data/pacman.png", "pacman");
    image_manager::load_img("./data/rival.png", "rival");
    image_manager::load_img("./data/bg.png", "bg");
    image_manager::load_img("./data/bg_red.png", "bg_red");
    image_manager::load_img("./data/bg_green.png", "bg_green");
    image_manager::load_img("./data/bg_blue.png", "bg_blue");
    image_manager::load_img("./data/food.png", "food");
    image_manager::load_img("./data/food_counter.png", "food_counter");
    image_manager::load_img("./data/akabei.png", "akabei");
    image_manager::load_img("./data/pinky.png", "pinky");
    image_manager::load_img("./data/aosuke.png", "aosuke");
    image_manager::load_img("./data/guzuta.png", "guzuta");
    image_manager::load_img("./data/mon_run.png", "mon_run");
    image_manager::load_img("./data/plate.png", "plate");
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
    input::update_input();
    switch (Game_state) {
      case game_state::title:
        title();
        break;
      case game_state::start:
        game_start();
        break;
      case game_state::playing:
        play_game();
        break;
      case game_state::clear:
        game_clear();
        break;
      case game_state::miss:
        game_miss();
        break;
      case game_state::gameover:
        game_over();
        break;
      case game_state::pause:
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
  // TODO: narrow scope
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
      wipe::set_wipe_in();
      wipe::draw_wipe(SCREEN_WIDTH);
      ++Game_count;
      break;
    case 1:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");
      wipe::draw_wipe(SCREEN_WIDTH);
      if (wipe::update_wipe()) {
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

      if (Edge_key[0][input_device::x] || Edge_key[1][input_device::x] ||
          Edge_key[0][input_device::space]) {
        ++Game_count;
        Blink_count = 0;
      }
      break;
    case 3:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");
      if (!Press_key[0][input_device::x] && !Press_key[1][input_device::x] &&
          !Press_key[0][input_device::space]) {
        ++Game_count;
      }
      break;
    case 4:
      Kanji_PutText(Screen, 230, 180, Font[FONT_SIZE_24], BLACK,
                    "P a c - M a n");

      switch (Game_mode) {
        case game_mode::single:
          dst_select.x = 250;
          dst_select.y = 300;
          dst_select.w = 96;
          dst_select.h = 16;
          SDL_FillRect(Screen, &dst_select, black);
          Kanji_PutText(Screen, 270, 300, Font[FONT_SIZE_16], WHITE, "1P MODE");
          Kanji_PutText(Screen, 270, 350, Font[FONT_SIZE_16], BLACK, "VS MODE");
          break;
        case game_mode::battle:
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

      if (Press_key[0][input_device::x] || Press_key[1][input_device::x] ||
          Press_key[0][input_device::space]) {
        wipe::set_wipe_out();
        wipe::draw_wipe(SCREEN_WIDTH);
        ++Game_count;
      }

      if (Press_key[0][input_device::bUTTON_2] ||
          Press_key[1][input_device::bUTTON_2]) {
        Game_count -= 2;
        Game_mode = game_mode::single;
      }

      if (Press_key[0][input_device::down] || Press_key[1][input_device::down]) {
        Game_mode = game_mode::battle;
      } else if (Press_key[0][input_device::up] || Press_key[1][input_device::up]) {
        Game_mode = game_mode::single;
      }
      break;
    case 5:
      switch (Game_mode) {
        case game_mode::single:
          dst_select.x = 250;
          dst_select.y = 300;
          dst_select.w = 96;
          dst_select.h = 16;
          SDL_FillRect(Screen, &dst_select, black);
          Kanji_PutText(Screen, 270, 300, Font[FONT_SIZE_16], WHITE, "1P MODE");
          Kanji_PutText(Screen, 270, 350, Font[FONT_SIZE_16], BLACK, "VS MODE");
          break;
        case game_mode::battle:
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

      wipe::draw_wipe(SCREEN_WIDTH);

      // initialize globals
      if (wipe::update_wipe()) {
        map::init_map();
        food::init_food();
        main_chara::init_main_chara();
        enemy::init_enemy();

        Game_count = 0;
        Game_state = game_state::start;
        Game_level = 1;
        Main_chara_life = 2;

        if (Game_mode == game_mode::battle) {
          Num_player = 2;
        }

        for (int i = 0; i < Num_player; ++i) {
          Now_score[i] = 0;
        }

        srand((unsigned int)time(nullptr));
      }
      break;
    default:
      cerr << "error: undefined game count." << '\n';
      break;
  }
}

void game_start() {
  map::draw_map();
  food::draw_food();
  enemy::draw_enemy();
  main_chara::draw_main_chara();
  draw_score();
  switch (Game_count) {
    case 0:
      if ((Main_chara_life == 2) && (Rival_chara_life == 2)) {
        Mix_PlayMusic(Music[2], 0);
      }
      wipe::set_wipe_in();
      wipe::draw_wipe(OFFSET_X);
      ++Game_count;
      break;
    case 1:
      wipe::draw_wipe(OFFSET_X);
      if (wipe::update_wipe()) {
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
    Game_state = game_state::playing;
    for (int i = 0; i < Num_player; ++i) {
      Power_chara_mode[i] = 0;
    }
    for (int i = 0; i < enemy_character::count; ++i) {
      Enemy_state[i] = enemy_state::normal;
    }
  }
}

void play_game() {
  map::draw_map();
  food::draw_food();
  enemy::draw_enemy();
  main_chara::draw_main_chara();
  draw_score();
  main_chara::move_main_chara();
  for (int i = 0; i < enemy_character::count; ++i) {
    if (Enemy_run_debug || (Enemy_state[i] == enemy_state::lose)) {
      enemy::move_lose_enemy(i);
    } else {
      enemy::move_normal_enemy(i);
    }
  }
  food::check_food_state();
  enemy::check_hit_enemy();

  if (Edge_key[0][input_device::space]) {
    Game_state = game_state::pause;
  }

  if (Edge_key[0][input_device::b]) {
    Enemy_run_debug = !Enemy_run_debug;
  }
}

void game_clear() {
  map::draw_map();
  food::draw_food();
  enemy::draw_enemy();
  main_chara::draw_main_chara();
  draw_score();

  if (Game_count == 0) {
    wipe::set_wipe_out();
    wipe::draw_wipe(OFFSET_X);
    ++Game_count;
  } else if (Game_count == 1) {
    wipe::draw_wipe(OFFSET_X);
    if (wipe::update_wipe()) {
      if (Game_level >= 256) {
        Game_count = 0;
        Game_state = game_state::gameover;
      } else {
        Game_count = 0;
        Game_state = game_state::start;
        ++Game_level;
        food::init_food();
        enemy::init_enemy();
        main_chara::init_main_chara();
      }
    }
  } // TODO: else
}

void game_miss() {
  map::draw_map();
  food::draw_food();
  enemy::draw_enemy();
  main_chara::draw_main_chara();
  draw_score();

  if (Game_count == 0) {
    Mix_PlayMusic(Music[3], 0);
    wipe::set_wipe_out();
    if ((Main_chara_life == 0) || (Rival_chara_life == 0)) {
      wipe::draw_wipe(SCREEN_WIDTH);
    } else {
      wipe::draw_wipe(OFFSET_X);
    }
    ++Game_count;
  } else if (Game_count == 1) {
    if ((Main_chara_life == 0) || (Rival_chara_life == 0)) {
      wipe::draw_wipe(SCREEN_WIDTH);
    } else {
      wipe::draw_wipe(OFFSET_X);
    }

    if (Choice_hit) {
      main_chara::add_main_chara_pos(0, -1);
      if (wipe::update_wipe()) {
        --Main_chara_life;
        if (Main_chara_life >= 0) {
          Game_count = 0;
          Game_state = game_state::start;
          enemy::init_enemy();
          main_chara::init_main_chara();
        } else {
          Game_count = 0;
          Blink_count = 0;
          Game_state = game_state::gameover;
        }
      }
    } else {
      main_chara::add_rival_chara_pos(0, -1);
      if (wipe::update_wipe()) {
        --Rival_chara_life;
        if (Rival_chara_life >= 0) {
          Game_count = 0;
          Game_state = game_state::start;
          enemy::init_enemy();
          main_chara::init_main_chara();
        } else {
          Game_count = 0;
          Blink_count = 0;
          Game_state = game_state::gameover;
        }
      }
    }
  } // TODO: else
}

void game_over() {
  SDL_Rect dst_back = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  Uint32 white = 0xffffffff;
  SDL_FillRect(Screen, &dst_back, white);

  switch (Game_mode) {
    case game_mode::single:
      switch (Game_count) {
        case 0:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          wipe::set_wipe_in();
          wipe::draw_wipe(SCREEN_WIDTH);
          ++Game_count;
          break;
        case 1:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          wipe::draw_wipe(SCREEN_WIDTH);
          if (wipe::update_wipe()) {
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

          if (Press_key[0][input_device::x] || Press_key[1][input_device::x] ||
              Press_key[0][input_device::space]) {
            ++Game_count;
            wipe::set_wipe_out();
            wipe::draw_wipe(SCREEN_WIDTH);
          }
          break;
        case 3:
          wipe::draw_wipe(SCREEN_WIDTH);
          if (wipe::update_wipe()) {
            Blink_count = 0;
            Game_count = 0;
            Game_state = game_state::title;
          }
          break;
        default:
          cerr << "error: undefined game count." << '\n';
          break;
      }
      break;
    case game_mode::battle:
      switch (Game_count) {
        case 0:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          wipe::set_wipe_in();
          wipe::draw_wipe(SCREEN_WIDTH);
          ++Game_count;
          break;
        case 1:
          Kanji_PutText(Screen, 210, 180, Font[FONT_SIZE_24], RED,
                        "G a m e O v e r");
          wipe::draw_wipe(SCREEN_WIDTH);
          if (wipe::update_wipe()) {
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

          if (Press_key[0][input_device::x] || Press_key[1][input_device::x] ||
              Press_key[0][input_device::space]) {
            ++Game_count;
            wipe::set_wipe_out();
            wipe::draw_wipe(SCREEN_WIDTH);
          }
          break;
        case 3:
          wipe::draw_wipe(SCREEN_WIDTH);
          if (wipe::update_wipe()) {
            Blink_count = 0;
            Game_count = 0;
            Rival_chara_life = 2;
            Game_state = game_state::title;
          }
          break;
        default:
          // TODO: exit or throw exception
          cerr << "error: undefined game count." << '\n';
          break;
      }
      break;
    default:
      // TODO: exit or throw exception
      cerr << "error: undefined game state." << '\n';
      break;
  }
}

void game_pause() {
  map::draw_map();
  food::draw_food();
  enemy::draw_enemy();
  main_chara::draw_main_chara();
  draw_score();
  draw_translucence();
  if (Edge_key[0][input_device::space]) {
    Game_state = game_state::playing;
  }
}

// TODO: reduce magic numbers
// TODO: delete meanigneless block scopes
void draw_score() {
  {
    SDL_Surface *p_surface = image_manager::get_img("plate");
    SDL_Rect dst;
    dst.x = OFFSET_X;
    dst.y = 0;
    SDL_BlitSurface(p_surface, nullptr, Screen, &dst);
  }
  {
    Kanji_PutText(Screen, OFFSET_X + 10, SCREEN_HEIGHT / 6, Font[FONT_SIZE_16],
                  WHITE, "Score: %6d", Now_score[0]);
    SDL_Surface *p_surface = image_manager::get_img("pacman");
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
    if (Game_mode == game_mode::battle) {
      Kanji_PutText(Screen, OFFSET_X + 10, SCREEN_HEIGHT / 6 + 80,
                    Font[FONT_SIZE_16], WHITE, "Score: %6d", Now_score[1]);
      SDL_Surface *p_surface = image_manager::get_img("rival");
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
  image_manager::del_all_img();
  for (int i = 0; i < NUM_FONT; ++i) {
    Kanji_CloseFont(Font[i]);
  }
  input::end_joystick();
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

// TODO: utils.cpp utils.hpp
int get_distance(int x1, int y1, int x2, int y2) {
  return (int)hypot((double)(x1 - x2), (double)(y1 - y2));
}

void draw_translucence() {
  Uint32 rmask, gmask, bmask, amask;
  Uint8 alpha = 128;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  SDL_Rect dst_back;
  dst_back.x = 0;
  dst_back.y = 0;

  SDL_Surface *trans_surface = SDL_CreateRGBSurface(
      SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);
  if (!trans_surface) {
    cerr << "CreateRGBSurface failed: " <<  SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(trans_surface, nullptr, Screen, &dst_back);
  if (Blink_count < 30) {
    Kanji_PutText(Screen, 240, 200, Font[FONT_SIZE_24], WHITE, "P a u s e");
    ++Blink_count;
  } else if (Blink_count < 60) {
    ++Blink_count;
  } else {
    Blink_count = 0;
  }
}