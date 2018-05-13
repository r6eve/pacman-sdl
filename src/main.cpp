#define MAIN

#include <SDL/SDL_mixer.h>

#include <getopt.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "def_global.hpp"
#include "enemy.hpp"
#include "font_manager.hpp"
#include "food.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "map.hpp"
#include "mixer_manager.hpp"
#include "player.hpp"
#include "wipe.hpp"

using namespace std;

namespace {

unsigned int Game_level;
game_state Game_state;
unsigned int Blink_count;
unsigned int Game_count;
bool Debug_lose_enemy;

}  // namespace

bool parse_options(const int argc, char **argv) noexcept;
void init(const bool debug_mode) noexcept;
void init_sdl(const bool debug_mode);
void main_loop(const bool debug_mode) noexcept;
void game_title(Wipe &wipe, Food &food, Enemy &enemy, Player &player1,
                Player &player2) noexcept;
void game_start(Wipe &wipe, Food &food, const Enemy &enemy, Player &player1,
                Player &player2) noexcept;
void play_game(Food &food, Enemy &enemy, Player &player1,
               Player &player2) noexcept;
void game_clear(Wipe &wipe, Food &food, Enemy &enemy, Player &player1,
                Player &player2) noexcept;
void game_miss(Wipe &wipe, Food &food, Enemy &enemy, Player &player1,
               Player &player2) noexcept;
void game_over(Wipe &wipe, const Player &player1,
               const Player &player2) noexcept;
void game_pause(Food &food, const Enemy &enemy, Player &player1,
                Player &player2) noexcept;
// TODO: make enum class `font_type` and `color`
void draw_text(const unsigned char font_size, Uint8 r, Uint8 g, Uint8 b, int x,
               int y, const char *str) noexcept;
void draw_score(Player &player1, Player &player2) noexcept;
bool poll_event() noexcept;
void wait_game() noexcept;
void draw_fps() noexcept;
void end() noexcept;
void draw_translucence() noexcept;

int main(int argc, char **argv) {
  const bool debug_mode = parse_options(argc, argv);
  init(debug_mode);
  main_loop(debug_mode);
  end();
  exit(EXIT_SUCCESS);
}

bool parse_options(const int argc, char **argv) noexcept {
  bool ret = false;
  opterr = 0;
  const option long_options[] = {
      {"debug", no_argument, nullptr, 'd'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0},
  };

  for (;;) {
    const int curind = optind;
    const int c = getopt_long(argc, argv, "dh", long_options, nullptr);
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

void init(const bool debug_mode) noexcept {
  try {
    init_sdl(debug_mode);
    Font_manager::init();
    Image_manager::init();
    Mixer_manager::init();
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    exit(EXIT_FAILURE);
  }

  Input_manager::init_joystick();

  // initialize global variables
  Blink_count = 0;
  Debug_lose_enemy = false;
  Game_count = 0;
  Game_mode = game_mode::single;
  Game_state = game_state::title;
}

void init_sdl(const bool debug_mode) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw SDL_GetError();
  }

  SDL_WM_SetCaption("pacman-sdl", nullptr);
  if (debug_mode) {
    Screen = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                              SDL_HWSURFACE | SDL_DOUBLEBUF);
  } else {
    Screen = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                              SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  }
  if (!Screen) {
    throw SDL_GetError();
  }

  SDL_ShowCursor(SDL_DISABLE);
}

void main_loop(const bool debug_mode) noexcept {
  Wipe wipe;
  Food food;
  Enemy enemy;
  Player player1(0);
  Player player2(1);
  for (;;) {
    Input_manager::update(debug_mode);
    switch (Game_state) {
      case game_state::title:
        game_title(wipe, food, enemy, player1, player2);
        break;
      case game_state::start:
        game_start(wipe, food, enemy, player1, player2);
        break;
      case game_state::playing:
        play_game(food, enemy, player1, player2);
        break;
      case game_state::clear:
        game_clear(wipe, food, enemy, player1, player2);
        break;
      case game_state::miss:
        game_miss(wipe, food, enemy, player1, player2);
        break;
      case game_state::gameover:
        game_over(wipe, player1, player2);
        break;
      case game_state::pause:
        game_pause(food, enemy, player1, player2);
        break;
      default:
        // NOTREACHED
        break;
    }
    if (!poll_event()) {
      return;
    }
    if (debug_mode) {
      draw_fps();
    }
    SDL_Flip(Screen);
    wait_game();
  }
}

void game_title(Wipe &wipe, Food &food, Enemy &enemy, Player &player1,
                Player &player2) noexcept {
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
      draw_text(36, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");
      wipe.draw(screen::width);
      if (wipe.update()) {
        ++Game_count;
      }
      break;
    }
    case 2: {
      draw_text(36, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");
      if (Blink_count < 30) {
        draw_text(16, 0x00, 0x00, 0x00, 205, 300,
                  "P r e s s   S p a c e   K e y");
        ++Blink_count;
      } else if (Blink_count < 60) {
        ++Blink_count;
      } else {
        Blink_count = 0;
      }

      if (Input_manager::edge_key_p(0, input_device::x) ||
          Input_manager::edge_key_p(1, input_device::x) ||
          Input_manager::edge_key_p(0, input_device::space)) {
        ++Game_count;
        Blink_count = 0;
      }
      break;
    }
    case 3: {
      draw_text(36, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");
      if (!Input_manager::press_key_p(0, input_device::x) &&
          !Input_manager::press_key_p(1, input_device::x) &&
          !Input_manager::press_key_p(0, input_device::space)) {
        ++Game_count;
      }
      break;
    }
    case 4: {
      draw_text(36, 0x00, 0x00, 0x00, 160, 160, "P  a  c  -  M  a  n");

      switch (Game_mode) {
        case game_mode::single: {
          SDL_Rect dst = {250, 298, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(16, 0xff, 0xff, 0xff, 270, 300, "1P MODE");
          draw_text(16, 0x00, 0x00, 0x00, 270, 350, "VS MODE");
          break;
        }
        case game_mode::battle: {
          SDL_Rect dst = {250, 348, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(16, 0x00, 0x00, 0x00, 270, 300, "1P MODE");
          draw_text(16, 0xff, 0xff, 0xff, 270, 350, "VS MODE");
          break;
        }
        default:
          // NOTREACHED
          break;
      }

      if (Input_manager::press_key_p(0, input_device::x) ||
          Input_manager::press_key_p(1, input_device::x) ||
          Input_manager::press_key_p(0, input_device::space)) {
        wipe.set_wipe_out();
        wipe.draw(screen::width);
        ++Game_count;
      }

      if (Input_manager::press_key_p(0, input_device::button_2) ||
          Input_manager::press_key_p(1, input_device::button_2)) {
        Game_count -= 2;
        Game_mode = game_mode::single;
      }

      if (Input_manager::press_key_p(0, input_device::down) ||
          Input_manager::press_key_p(1, input_device::down)) {
        Game_mode = game_mode::battle;
      } else if (Input_manager::press_key_p(0, input_device::up) ||
                 Input_manager::press_key_p(1, input_device::up)) {
        Game_mode = game_mode::single;
      }
      break;
    }
    case 5: {
      switch (Game_mode) {
        case game_mode::single: {
          SDL_Rect dst = {250, 298, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(16, 0xff, 0xff, 0xff, 270, 300, "1P MODE");
          draw_text(16, 0x00, 0x00, 0x00, 270, 350, "VS MODE");
          break;
        }
        case game_mode::battle: {
          SDL_Rect dst = {250, 348, 112, 26};
          SDL_FillRect(Screen, &dst, 0x00000000);
          draw_text(16, 0x00, 0x00, 0x00, 270, 300, "1P MODE");
          draw_text(16, 0xff, 0xff, 0xff, 270, 350, "VS MODE");
          break;
        }
        default:
          // NOTREACHED
          break;
      }

      wipe.draw(screen::width);

      // initialize globals
      if (wipe.update()) {
        Map::init();
        food.init();
        enemy.init();
        player1.init_pos();
        player2.init_pos();
        player1.set_life(2);
        player2.set_life(2);
        player1.set_score(0);
        player2.set_score(0);
        player1.set_damaged(false);
        player2.set_damaged(false);
        player1.set_power_mode(0);
        player2.set_power_mode(0);

        Game_count = 0;
        Game_state = game_state::start;
        Game_level = 1;

        srand((unsigned int)time(nullptr));
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void game_start(Wipe &wipe, Food &food, const Enemy &enemy, Player &player1,
                Player &player2) noexcept {
  Map::draw(Game_level);
  food.draw();
  enemy.draw();
  player1.draw();
  player2.draw();
  draw_score(player1, player2);
  switch (Game_count) {
    case 0: {
      // TODO: Is it correct?
      if ((player1.get_life() == 2) && (player2.get_life() == 2)) {
        Mix_PlayMusic(Mixer_manager::get_music("beginning"), 0);
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
    draw_text(36, 0xff, 0x00, 0x00, 153, 170, ss.str().c_str());
  } else if (Game_count < 200) {
    draw_text(36, 0xff, 0x00, 0x00, 165, 170, "S t a r t");
  }

  if (Game_count > 220) {
    Game_count = 0;
    Game_state = game_state::playing;
    player1.set_power_mode(0);
    player2.set_power_mode(0);
  }
}

void play_game(Food &food, Enemy &enemy, Player &player1,
               Player &player2) noexcept {
  Map::draw(Game_level);
  food.draw();
  enemy.draw();
  player1.draw();
  player2.draw();
  draw_score(player1, player2);
  enemy.move(Debug_lose_enemy, player1, player2);
  player1.move();
  player2.move();

  // すべてのエサ取得と敵衝突が同時なら，すべてのエサ取得を優先しクリアへ
  const bool food_state = food.check_state(player1, player2);
  const bool hit_enemy = enemy.check_hit_enemy(player1, player2);
  if (food_state) {
    Game_state = game_state::clear;
  } else if (hit_enemy) {
    Game_state = game_state::miss;
  }

  if (Input_manager::edge_key_p(0, input_device::space)) {
    Game_state = game_state::pause;
  }

  if (Input_manager::edge_key_p(0, input_device::b)) {
    Debug_lose_enemy = !Debug_lose_enemy;
  }
}

void game_clear(Wipe &wipe, Food &food, Enemy &enemy, Player &player1,
                Player &player2) noexcept {
  Map::draw(Game_level);
  food.draw();
  enemy.draw();
  player1.draw();
  player2.draw();
  draw_score(player1, player2);

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
      player1.init_pos();
      player2.init_pos();
    }
  }
}

void game_miss(Wipe &wipe, Food &food, Enemy &enemy, Player &player1,
               Player &player2) noexcept {
  Map::draw(Game_level);
  food.draw();
  enemy.draw();
  player1.draw();
  player2.draw();
  draw_score(player1, player2);

  if (Game_count == 0) {
    Mix_PlayMusic(Mixer_manager::get_music("death"), 0);
    wipe.set_wipe_out();
    if ((player1.get_life() == 0) || (player2.get_life() == 0)) {
      wipe.draw(screen::width);
    } else {
      wipe.draw(screen::offset_x);
    }
    ++Game_count;
    return;
  }

  if ((player1.get_life() == 0) || (player2.get_life() == 0)) {
    wipe.draw(screen::width);
  } else {
    wipe.draw(screen::offset_x);
  }

  // TODO: use pointer to delete if-clauses
  if (player1.get_damaged()) {
    Point pos = player1.get_pos();
    player1.set_pos(Point{pos.x, pos.y - 1});
    if (wipe.update()) {
      const int life = player1.get_life() - 1;
      player1.set_life(life);
      if (life >= 0) {
        Game_count = 0;
        Game_state = game_state::start;
        enemy.init();
        player1.init_pos();
        player2.init_pos();
        player1.set_damaged(false);
        player2.set_damaged(false);
      } else {
        Game_count = 0;
        Blink_count = 0;
        Game_state = game_state::gameover;
      }
    }
  } else {
    Point pos = player2.get_pos();
    player2.set_pos(Point{pos.x, pos.y - 1});
    if (wipe.update()) {
      const int life = player2.get_life() - 1;
      player2.set_life(life);
      if (life >= 0) {
        Game_count = 0;
        Game_state = game_state::start;
        enemy.init();
        player1.init_pos();
        player2.init_pos();
        player1.set_damaged(false);
        player2.set_damaged(false);
      } else {
        Game_count = 0;
        Blink_count = 0;
        Game_state = game_state::gameover;
      }
    }
  }
}

void game_over(Wipe &wipe, const Player &player1,
               const Player &player2) noexcept {
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_FillRect(Screen, &dst, 0xffffffff);

  switch (Game_mode) {
    case game_mode::single: {
      switch (Game_count) {
        case 0: {
          draw_text(36, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.set_wipe_in();
          wipe.draw(screen::width);
          ++Game_count;
          break;
        }
        case 1: {
          draw_text(36, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.draw(screen::width);
          if (wipe.update()) {
            ++Game_count;
          }
          break;
        }
        case 2: {
          draw_text(36, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          stringstream ss;
          ss << "Y o u r  S c o r e   " << player1.get_score();
          draw_text(36, 0x00, 0x00, 0x00, 120, 220, ss.str().c_str());

          if (Blink_count < 30) {
            draw_text(16, 0x00, 0x00, 0x00, 210, 350,
                      "P r e s s  S p a c e  K e y");
            ++Blink_count;
          } else if (Blink_count < 60) {
            ++Blink_count;
          } else {
            Blink_count = 0;
          }

          if (Input_manager::press_key_p(0, input_device::x) ||
              Input_manager::press_key_p(1, input_device::x) ||
              Input_manager::press_key_p(0, input_device::space)) {
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
          // NOTREACHED
          break;
      }
      break;
    }
    case game_mode::battle: {
      switch (Game_count) {
        case 0: {
          draw_text(36, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.set_wipe_in();
          wipe.draw(screen::width);
          ++Game_count;
          break;
        }
        case 1: {
          draw_text(36, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          wipe.draw(screen::width);
          if (wipe.update()) {
            ++Game_count;
          }
          break;
        }
        case 2: {
          draw_text(36, 0xff, 0x00, 0x00, 165, 100, "G a m e O v e r");
          stringstream ss;
          const unsigned int p1_score = player1.get_score();
          const unsigned int p2_score = player2.get_score();
          if (p1_score > p2_score) {
            ss << "1 P  W I N  " << p1_score;
            draw_text(36, 0x00, 0x00, 0x00, 170, 240, ss.str().c_str());
          } else if (p1_score < p2_score) {
            ss << "2 P  W I N  " << p2_score;
            draw_text(36, 0x00, 0x00, 0x00, 170, 240, ss.str().c_str());
          } else {
            ss << "D R A W  " << p1_score;
            draw_text(36, 0x00, 0x00, 0x00, 170, 240, ss.str().c_str());
          }

          if (Blink_count < 30) {
            draw_text(16, 0x00, 0x00, 0x00, 210, 380,
                      "P r e s s  S p a c e  K e y");
            ++Blink_count;
          } else if (Blink_count < 60) {
            ++Blink_count;
          } else {
            Blink_count = 0;
          }

          if (Input_manager::press_key_p(0, input_device::x) ||
              Input_manager::press_key_p(1, input_device::x) ||
              Input_manager::press_key_p(0, input_device::space)) {
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
          // NOTREACHED
          break;
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void game_pause(Food &food, const Enemy &enemy, Player &player1,
                Player &player2) noexcept {
  Map::draw(Game_level);
  food.draw();
  enemy.draw();
  player1.draw();
  player2.draw();
  draw_score(player1, player2);
  draw_translucence();
  if (Input_manager::edge_key_p(0, input_device::space)) {
    Game_state = game_state::playing;
  }
}

void draw_text(const unsigned char font_size, Uint8 r, Uint8 g, Uint8 b, int x,
               int y, const char *str) noexcept {
  SDL_Color black = {r, g, b, 0};
  SDL_Surface *font_surface =
      TTF_RenderUTF8_Blended(Font_manager::get(font_size), str, black);
  SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),
                  static_cast<Uint16>(font_surface->h)};
  SDL_Rect dst = {static_cast<Sint16>(x), static_cast<Sint16>(y), 0, 0};
  SDL_BlitSurface(font_surface, &src, Screen, &dst);
}

// TODO: reduce magic numbers
void draw_score(Player &player1, Player &player2) noexcept {
  {
    SDL_Surface *p_surface = Image_manager::get("plate");
    SDL_Rect dst = {screen::offset_x, 0, 0, 0};
    SDL_BlitSurface(p_surface, nullptr, Screen, &dst);
  }
  {
    stringstream score;
    score << "S c o r e  :  " << setw(6) << player1.get_score();
    draw_text(16, 0xff, 0xff, 0xff, screen::offset_x + 20,
              screen::height / 7 + 10, score.str().c_str());
    SDL_Surface *p_surface = Image_manager::get("player1");
    SDL_Rect src = {block::size, 0, block::size, block::size};
    SDL_Rect dst = {screen::offset_x + 60, (screen::height / 6 + 32) - 5, 0, 0};
    SDL_BlitSurface(p_surface, &src, Screen, &dst);
    stringstream life;
    life << "x  " << player1.get_life();
    draw_text(16, 0xff, 0xff, 0xff, screen::offset_x + 90,
              screen::height / 7 + 40, life.str().c_str());
    if (Game_mode == game_mode::battle) {
      stringstream score;
      score << "S c o r e  :  " << setw(6) << player2.get_score();
      draw_text(16, 0xff, 0xff, 0xff, screen::offset_x + 20,
                screen::height / 7 + 90, score.str().c_str());
      SDL_Surface *p_surface = Image_manager::get("player2");
      SDL_Rect src = {block::size, 0, block::size, block::size};
      SDL_Rect dst = {screen::offset_x + 60, (screen::height / 6 + 112) - 5, 0,
                      0};
      SDL_BlitSurface(p_surface, &src, Screen, &dst);
      stringstream life;
      life << "x  " << player2.get_life();
      draw_text(16, 0xff, 0xff, 0xff, screen::offset_x + 90,
                screen::height / 7 + 122, life.str().c_str());
    }
  }
  {
    if (player1.get_power_mode()) {
      SDL_Rect dst = {screen::offset_x + 10, screen::height / 6 * 4,
                      static_cast<Uint16>(player1.get_power_mode() / 4),
                      block::size};
      SDL_FillRect(Screen, &dst, 0xffff00);
      player1.set_power_mode(player1.get_power_mode() - 1);
    }
    if (player2.get_power_mode()) {
      SDL_Rect dst = {screen::offset_x + 10, screen::height / 6 * 4 + 30,
                      static_cast<Uint16>(player2.get_power_mode() / 4),
                      block::size};
      SDL_FillRect(Screen, &dst, 0x808080);
      player2.set_power_mode(player2.get_power_mode() - 1);
    }
  }
}

bool poll_event() noexcept {
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

void wait_game() noexcept {
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

void draw_fps() noexcept {
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
    draw_text(16, 0xff, 0xff, 0xff, screen::offset_x + 15, 16,
              ss.str().c_str());
  }
  pre_count = now_count;
}

void end() noexcept {
  Font_manager::end();
  Image_manager::end();
  Input_manager::end_joystick();
  Mixer_manager::end();
  atexit(SDL_Quit);
}

void draw_translucence() noexcept {
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
    draw_text(36, 0xff, 0xff, 0xff, 165, 170, "P a u s e");
    ++Blink_count;
  } else if (Blink_count < 60) {
    ++Blink_count;
  } else {
    Blink_count = 0;
  }
}
