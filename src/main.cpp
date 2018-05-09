#define MAIN

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <getopt.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "def_global.hpp"
#include "enemy.hpp"
#include "food.hpp"
#include "image_manager.hpp"
#include "input.hpp"
#include "map.hpp"
#include "player.hpp"
#include "util.hpp"
#include "wipe.hpp"

using namespace std;

namespace {

game_state Game_state;
TTF_Font *Ttf_fonts[2];
unsigned int Blink_count;
unsigned int Game_count;
unsigned int Num_player;
bool Enemy_run_debug;

}  // namespace

bool parse_options(const int argc, char **argv);
void init();
// TODO: use those in each constructor
void init_sdl();
void init_font();
void init_img();
void init_music();
void main_loop();
void game_title(Wipe &wipe, Food &food, Enemy &enemy);
void game_start(Wipe &wipe, Food &food, Enemy &enemy);
void play_game(Food &food, Enemy &enemy);
void game_clear(Wipe &wipe, Food &food, Enemy &enemy);
void game_miss(Wipe &wipe, Food &food, Enemy &enemy);
void game_over(Wipe &wipe);
void game_pause(Food &food, Enemy &enemy);
// TODO: make enum class `font_type` and `color`
void draw_text(int font_type, Uint8 r, Uint8 g, Uint8 b, int x, int y,
               const char *str);
void draw_score();
bool poll_event();
void wait_game();
void draw_fps();
void end();
void draw_translucence();

int main(int argc, char **argv) {
  Debug_mode = parse_options(argc, argv);
  init();
  main_loop();
  end();
  exit(EXIT_SUCCESS);
}

bool parse_options(const int argc, char **argv) {
  bool ret = false;
  opterr = 0;
  const option long_options[] = {
      {"debug", no_argument, nullptr, 'd'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0},
  };

  int c;
  for (;;) {
    int curind = optind;
    c = getopt_long(argc, argv, "dh", long_options, nullptr);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'h':
        cout << R"(Usage: pacman-sdl [options]

Options:
    -d  --debug         debug mode
    -h, --help          print this help menu
)";
        exit(EXIT_SUCCESS);
      case 'd':
        ret = true;
        break;
      case '?': {
        string av(argv[curind]);
        int n = 0;
        while (av[n] == '-') {
          ++n;
        }
        av.erase(av.begin(), av.begin() + n);
        cerr << "Unrecognized option: '" << av << "'\n";
        exit(EXIT_FAILURE);
      }
      default:
        // do nothing
        break;
    }
  }

  return ret;
}

void init() {
  try {
    init_sdl();
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    atexit(SDL_Quit);
    exit(EXIT_FAILURE);
  }

  try {
    init_font();
    init_img();
    init_music();
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    end();
    exit(EXIT_FAILURE);
  }

  Input::init_joystick();

  // initialize global variables
  Blink_count = 0;
  Enemy_run_debug = false;
  Game_count = 0;
  Game_mode = game_mode::single;
  Game_state = game_state::title;
}

void init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw SDL_GetError();
  }
  SDL_WM_SetCaption("pacman-sdl", nullptr);
  if (Debug_mode) {
    Screen = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                              SDL_HWSURFACE | SDL_DOUBLEBUF);
  } else {
    Screen = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                              SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  }
  if (!Screen) {
    throw SDL_GetError();
  }

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
    throw Mix_GetError();
  }

  if (TTF_Init() != 0) {
    throw TTF_GetError();
  }

  SDL_ShowCursor(SDL_DISABLE);
}

void init_font() {
  Ttf_fonts[0] = TTF_OpenFont("./data/GenEiGothicP-Heavy.otf", 36);
  Ttf_fonts[1] = TTF_OpenFont("./data/GenEiGothicP-Regular.otf", 16);
  if (!Ttf_fonts[0] || !Ttf_fonts[1]) {
    throw TTF_GetError();
  }
}

void init_img() {
  int flag = IMG_INIT_PNG;
  if (IMG_Init(flag) != flag) {
    throw IMG_GetError();
  }

  try {
    image_manager::load_image("./data/player1.png", "player1");
    image_manager::load_image("./data/player2.png", "player2");
    image_manager::load_image("./data/bg.png", "bg");
    image_manager::load_image("./data/bg_red.png", "bg_red");
    image_manager::load_image("./data/bg_green.png", "bg_green");
    image_manager::load_image("./data/bg_blue.png", "bg_blue");
    image_manager::load_image("./data/food.png", "food");
    image_manager::load_image("./data/food_counter.png", "food_counter");
    image_manager::load_image("./data/akabei.png", "akabei");
    image_manager::load_image("./data/pinky.png", "pinky");
    image_manager::load_image("./data/aosuke.png", "aosuke");
    image_manager::load_image("./data/guzuta.png", "guzuta");
    image_manager::load_image("./data/mon_run.png", "mon_run");
    image_manager::load_image("./data/plate.png", "plate");
  } catch (const char &e) {
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
  Wipe wipe;
  Food food;
  Enemy enemy;
  for (;;) {
    Input::update();
    switch (Game_state) {
      case game_state::title:
        game_title(wipe, food, enemy);
        break;
      case game_state::start:
        game_start(wipe, food, enemy);
        break;
      case game_state::playing:
        play_game(food, enemy);
        break;
      case game_state::clear:
        game_clear(wipe, food, enemy);
        break;
      case game_state::miss:
        game_miss(wipe, food, enemy);
        break;
      case game_state::gameover:
        game_over(wipe);
        break;
      case game_state::pause:
        game_pause(food, enemy);
        break;
      default:
        // NOTREACHED
        break;;
    }
    if (!poll_event()) {
      return;
    }
    if (Debug_mode) {
      draw_fps();
    }
    SDL_Flip(Screen);
    wait_game();
  }
}

void game_title(Wipe &wipe, Food &food, Enemy &enemy) {
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_FillRect(Screen, &dst, 0xffffffff);

  switch (Game_count) {
    case 0: {
      wipe.set_wipe_in();
      wipe.draw(screen::width);
      ++Game_count;
      break;
    }
    case 1: {
      draw_text(0, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");
      wipe.draw(screen::width);
      if (wipe.update()) {
        ++Game_count;
      }
      break;
    }
    case 2: {
      draw_text(0, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");
      if (Blink_count < 30) {
        draw_text(1, 0x00, 0x00, 0x00, 205, 300,
                  "P r e s s   S p a c e   K e y");
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
    }
    case 3: {
      draw_text(0, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");
      if (!Press_key[0][input_device::x] && !Press_key[1][input_device::x] &&
          !Press_key[0][input_device::space]) {
        ++Game_count;
      }
      break;
    }
    case 4: {
      draw_text(0, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");

      switch (Game_mode) {
        case game_mode::single: {
          SDL_Rect dst = {250, 298, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(1, 0xff, 0xff, 0xff, 270, 300, "1P MODE");
          draw_text(1, 0x00, 0x00, 0x00, 270, 350, "VS MODE");
          break;
        }
        case game_mode::battle: {
          SDL_Rect dst = {250, 348, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(1, 0x00, 0x00, 0x00, 270, 300, "1P MODE");
          draw_text(1, 0xff, 0xff, 0xff, 270, 350, "VS MODE");
          break;
        }
        default:
          cerr << "error: undefined game mode." << '\n';
          break;
      }

      if (Press_key[0][input_device::x] || Press_key[1][input_device::x] ||
          Press_key[0][input_device::space]) {
        wipe.set_wipe_out();
        wipe.draw(screen::width);
        ++Game_count;
      }

      if (Press_key[0][input_device::button_2] ||
          Press_key[1][input_device::button_2]) {
        Game_count -= 2;
        Game_mode = game_mode::single;
      }

      if (Press_key[0][input_device::down] ||
          Press_key[1][input_device::down]) {
        Game_mode = game_mode::battle;
      } else if (Press_key[0][input_device::up] ||
                 Press_key[1][input_device::up]) {
        Game_mode = game_mode::single;
      }
      break;
    }
    case 5: {
      switch (Game_mode) {
        case game_mode::single: {
          SDL_Rect dst = {250, 298, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(1, 0xff, 0xff, 0xff, 270, 300, "1P MODE");
          draw_text(1, 0x00, 0x00, 0x00, 270, 350, "VS MODE");
          break;
        }
        case game_mode::battle: {
          SDL_Rect dst = {250, 348, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(1, 0x00, 0x00, 0x00, 270, 300, "1P MODE");
          draw_text(1, 0xff, 0xff, 0xff, 270, 350, "VS MODE");
          break;
        }
        default:
          cerr << "error: undefined game mode."
               << "\n";
          break;
      }

      wipe.draw(screen::width);

      // initialize globals
      if (wipe.update()) {
        map::init();
        food.init();
        player::init();
        enemy.init();

        Game_count = 0;
        Game_state = game_state::start;
        Game_level = 1;
        player::set_player_1_life(2);  // TODO: set it in constructor
        player::set_player_2_life(2);

        Num_player = Game_mode == game_mode::single ? 1 : 2;

        for (unsigned int i = 0; i < Num_player; ++i) {
          Now_score[i] = 0;
        }

        srand((unsigned int)time(nullptr));
      }
      break;
    }
    default:
      cerr << "error: undefined game count." << '\n';
      break;
  }
}

void game_start(Wipe &wipe, Food &food, Enemy &enemy) {
  map::draw();
  food.draw();
  enemy.draw();
  player::draw();
  draw_score();
  switch (Game_count) {
    case 0: {
      if ((player::get_player_1_life() == 2) &&
          (player::get_player_2_life() == 2)) {
        Mix_PlayMusic(Music[2], 0);
      }
      wipe.set_wipe_in();
      wipe.draw(screen::offset_x);
      ++Game_count;
      break;
    }
    case 1: {
      wipe.draw(screen::offset_x);
      if (wipe.update()) {
        ++Game_count;
      }
      break;
    }
    default:
      ++Game_count;
      break;
  }
  if (Game_count < 130) {
    stringstream ss;
    ss << "S t a g e " << Game_level;
    draw_text(0, 0xff, 0x00, 0x00, 153, 170, ss.str().c_str());
  } else if (Game_count < 200) {
    draw_text(0, 0xff, 0x00, 0x00, 165, 170, "S t a r t");
  }

  if (Game_count > 220) {
    Game_count = 0;
    Game_state = game_state::playing;
    for (unsigned int i = 0; i < Num_player; ++i) {
      Power_chara_mode[i] = 0;
    }
    for (unsigned int i = 0; i < enemy_character::count; ++i) {
      Enemy_state[i] = enemy_state::normal;
    }
  }
}

void play_game(Food &food, Enemy &enemy) {
  map::draw();
  food.draw();
  enemy.draw();
  player::draw();
  draw_score();
  player::move();
  for (unsigned int i = 0; i < enemy_character::count; ++i) {
    if (Enemy_run_debug || (Enemy_state[i] == enemy_state::lose)) {
      enemy.move_lose_enemy(i);
    } else {
      enemy.move_normal_enemy(i);
    }
  }

  // すべてのエサ取得と敵衝突が同時なら，すべてのエサ取得を優先しクリアへ
  const bool food_state = food.check_state();
  const bool hit_enemy = enemy.check_hit_enemy();
  if (food_state) {
    Game_state = game_state::clear;
  } else if (hit_enemy) {
    Game_state = game_state::miss;
  }

  if (Edge_key[0][input_device::space]) {
    Game_state = game_state::pause;
  }

  if (Edge_key[0][input_device::b]) {
    Enemy_run_debug = !Enemy_run_debug;
  }
}

void game_clear(Wipe &wipe, Food &food, Enemy &enemy) {
  map::draw();
  food.draw();
  enemy.draw();
  player::draw();
  draw_score();

  if (Game_count == 0) {
    wipe.set_wipe_out();
    wipe.draw(screen::offset_x);
    ++Game_count;
    return;
  }

  wipe.draw(screen::offset_x);
  if (wipe.update()) {
    if (Game_level >= 256) {
      Game_count = 0;
      Game_state = game_state::gameover;
    } else {
      Game_count = 0;
      Game_state = game_state::start;
      ++Game_level;
      food.init();
      enemy.init();
      player::init();
    }
  }
}

void game_miss(Wipe &wipe, Food &food, Enemy &enemy) {
  map::draw();
  food.draw();
  enemy.draw();
  player::draw();
  draw_score();

  if (Game_count == 0) {
    Mix_PlayMusic(Music[3], 0);
    wipe.set_wipe_out();
    if ((player::get_player_1_life() == 0) ||
        (player::get_player_2_life() == 0)) {
      wipe.draw(screen::width);
    } else {
      wipe.draw(screen::offset_x);
    }
    ++Game_count;
    return;
  }

  if ((player::get_player_1_life() == 0) ||
      (player::get_player_2_life() == 0)) {
    wipe.draw(screen::width);
  } else {
    wipe.draw(screen::offset_x);
  }

  // TODO: use pointer to delete if-clauses
  if (Choice_hit) {
    player::add_player_1_pos(0, -1);
    if (wipe.update()) {
      const int life = player::get_player_1_life() - 1;
      player::set_player_1_life(life);
      if (life >= 0) {
        Game_count = 0;
        Game_state = game_state::start;
        enemy.init();
        player::init();
      } else {
        Game_count = 0;
        Blink_count = 0;
        Game_state = game_state::gameover;
      }
    }
  } else {
    player::add_player_2_pos(0, -1);
    if (wipe.update()) {
      const int life = player::get_player_2_life() - 1;
      player::set_player_2_life(life);
      if (life >= 0) {
        Game_count = 0;
        Game_state = game_state::start;
        enemy.init();
        player::init();
      } else {
        Game_count = 0;
        Blink_count = 0;
        Game_state = game_state::gameover;
      }
    }
  }
}

void game_over(Wipe &wipe) {
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_FillRect(Screen, &dst, 0xffffffff);

  switch (Game_mode) {
    case game_mode::single: {
      switch (Game_count) {
        case 0: {
          draw_text(0, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.set_wipe_in();
          wipe.draw(screen::width);
          ++Game_count;
          break;
        }
        case 1: {
          draw_text(0, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.draw(screen::width);
          if (wipe.update()) {
            ++Game_count;
          }
          break;
        }
        case 2: {
          draw_text(0, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          stringstream ss;
          ss << "Y o u r  S c o r e   " << Now_score[0];
          draw_text(0, 0x00, 0x00, 0x00, 120, 220, ss.str().c_str());

          if (Blink_count < 30) {
            draw_text(1, 0x00, 0x00, 0x00, 210, 350,
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
            wipe.set_wipe_out();
            wipe.draw(screen::width);
          }
          break;
        }
        case 3: {
          wipe.draw(screen::width);
          if (wipe.update()) {
            Blink_count = 0;
            Game_count = 0;
            Game_state = game_state::title;
          }
          break;
        }
        default:
          cerr << "error: undefined game count." << '\n';
          break;
      }
      break;
    }
    case game_mode::battle: {
      switch (Game_count) {
        case 0: {
          draw_text(0, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.set_wipe_in();
          wipe.draw(screen::width);
          ++Game_count;
          break;
        }
        case 1: {
          draw_text(0, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.draw(screen::width);
          if (wipe.update()) {
            ++Game_count;
          }
          break;
        }
        case 2: {
          draw_text(0, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          stringstream ss;
          if (Now_score[0] > Now_score[1]) {
            ss << "1 P  W I N  " << Now_score[0];
            draw_text(0, 0x00, 0x00, 0x00, 170, 240, ss.str().c_str());
          } else if (Now_score[1] > Now_score[0]) {
            ss << "2 P  W I N  " << Now_score[1];
            draw_text(0, 0x00, 0x00, 0x00, 170, 240, ss.str().c_str());
          } else {
            ss << "D R A W  " << Now_score[0];
            draw_text(0, 0x00, 0x00, 0x00, 170, 240, ss.str().c_str());
          }

          if (Blink_count < 30) {
            draw_text(1, 0x00, 0x00, 0x00, 210, 380,
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
            wipe.set_wipe_out();
            wipe.draw(screen::width);
          }
          break;
        }
        case 3: {
          wipe.draw(screen::width);
          if (wipe.update()) {
            Blink_count = 0;
            Game_count = 0;
            player::set_player_2_life(2);
            Game_state = game_state::title;
          }
          break;
        }
        default:
          // TODO: exit or throw exception
          cerr << "error: undefined game count." << '\n';
          break;
      }
      break;
    }
    default:
      // TODO: exit or throw exception
      cerr << "error: undefined game state." << '\n';
      break;
  }
}

void game_pause(Food &food, Enemy &enemy) {
  map::draw();
  food.draw();
  enemy.draw();
  player::draw();
  draw_score();
  draw_translucence();
  if (Edge_key[0][input_device::space]) {
    Game_state = game_state::playing;
  }
}

void draw_text(int font_type, Uint8 r, Uint8 g, Uint8 b, int x, int y,
               const char *str) {
  SDL_Color black = {r, g, b, 0};
  SDL_Surface *font_surface =
      TTF_RenderUTF8_Blended(Ttf_fonts[font_type], str, black);
  SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),
                  static_cast<Uint16>(font_surface->h)};
  SDL_Rect dst = {static_cast<Sint16>(x), static_cast<Sint16>(y), 0, 0};
  SDL_BlitSurface(font_surface, &src, Screen, &dst);
}

// TODO: reduce magic numbers
void draw_score() {
  {
    SDL_Surface *p_surface = image_manager::get_image("plate");
    SDL_Rect dst = {screen::offset_x, 0, 0, 0};
    SDL_BlitSurface(p_surface, nullptr, Screen, &dst);
  }
  {
    stringstream score;
    score << "S c o r e  :  " << setw(6) << Now_score[0];
    draw_text(1, 0xff, 0xff, 0xff, screen::offset_x + 20, screen::height / 7 + 10,
              score.str().c_str());
    SDL_Surface *p_surface = image_manager::get_image("player1");
    SDL_Rect src = {block::size, 0, block::size, block::size};
    SDL_Rect dst = {screen::offset_x + 60, (screen::height / 6 + 32) - 5, 0, 0};
    SDL_BlitSurface(p_surface, &src, Screen, &dst);
    stringstream life;
    life << "x  " << player::get_player_1_life();
    draw_text(1, 0xff, 0xff, 0xff, screen::offset_x + 90,
              screen::height / 7 + 40, life.str().c_str());
    if (Game_mode == game_mode::battle) {
      stringstream score;
      score << "S c o r e  :  " << setw(6) << Now_score[1];
      draw_text(1, 0xff, 0xff, 0xff, screen::offset_x + 20,
                screen::height / 7 + 90, score.str().c_str());
      SDL_Surface *p_surface = image_manager::get_image("player2");
      SDL_Rect src = {block::size, 0, block::size, block::size};
      SDL_Rect dst = {screen::offset_x + 60, (screen::height / 6 + 112) - 5, 0,
                      0};
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      stringstream life;
      life << "x  " << player::get_player_2_life();
      draw_text(1, 0xff, 0xff, 0xff, screen::offset_x + 90,
                screen::height / 7 + 122, life.str().c_str());
    }
  }
  {
    if (Power_chara_mode[0]) {
      SDL_Rect dst = {screen::offset_x + 10, screen::height / 6 * 4,
                      static_cast<Uint16>(Power_chara_mode[0] / 4),
                      block::size};
      SDL_FillRect(Screen, &dst, 0xffff00);
      --Power_chara_mode[0];
    }
    if (Power_chara_mode[1]) {
      SDL_Rect dst = {screen::offset_x + 10, screen::height / 6 * 4 + 30,
                      static_cast<Uint16>(Power_chara_mode[1] / 4),
                      block::size};
      SDL_FillRect(Screen, &dst, 0x808080);
      --Power_chara_mode[1];
    }
  }
}

bool poll_event() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return false;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }
        break;
      default:
        // do nothing
        break;
    }
  }
  return true;
}

void wait_game() {
  static Uint32 pre_count;
  const double wait_time = 1000.0 / screen::max_fps;
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

    stringstream ss;
    ss << "FrameRate[" << setprecision(2) << setiosflags(ios::fixed)
       << frame_rate << "]";
    draw_text(1, 0xff, 0xff, 0xff, screen::offset_x + 15, 16, ss.str().c_str());
  }
  pre_count = now_count;
}

void end() {
  for (unsigned int i = 0; i < 2; ++i) {
    TTF_CloseFont(Ttf_fonts[i]);
  }
  atexit(TTF_Quit);

  image_manager::delete_all_image();
  atexit(IMG_Quit);

  Input::end_joystick();

  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  // TODO: What's the number of 2?
  for (unsigned int i = 0; i < 2; ++i) {
    Mix_FreeMusic(Music[i]);
  }
  Mix_FreeChunk(Se[0]);
  Mix_CloseAudio();
  atexit(Mix_Quit);

  atexit(SDL_Quit);
}

void draw_translucence() {
  Uint32 rmask, gmask, bmask, amask;
  const Uint8 alpha = 128;
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
  SDL_Rect dst = {0, 0, 0, 0};
  SDL_Surface *trans_surface =
      SDL_CreateRGBSurface(SDL_SWSURFACE, screen::width, screen::height, 32,
                           rmask, gmask, bmask, amask);
  if (!trans_surface) {
    cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(trans_surface, nullptr, Screen, &dst);
  if (Blink_count < 30) {
    draw_text(0, 0xff, 0xff, 0xff, 165, 170, "P a u s e");
    ++Blink_count;
  } else if (Blink_count < 60) {
    ++Blink_count;
  } else {
    Blink_count = 0;
  }
}
