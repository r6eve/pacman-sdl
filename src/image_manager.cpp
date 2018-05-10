#include <SDL/SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "image_manager.hpp"

Image_manager::Img_list *Image_manager::img_list_top_ = nullptr;

void Image_manager::load_image(const char *path, const char *name) {
  SDL_Surface *img = IMG_Load(path);
  if (!img) {
    throw IMG_GetError();
  }

  Img_list *list = new Img_list;
  if (!list) {
    SDL_FreeSurface(img);
    throw "out of memory";
  }
  strcpy(list->name, name);
  list->img = img;
  list->prev = nullptr;
  list->next = img_list_top_;
  if (img_list_top_) {
    img_list_top_->prev = list;
  }
  img_list_top_ = list;
}

void Image_manager::init_image() {
  const int flag = IMG_INIT_PNG;
  if (IMG_Init(flag) != flag) {
    throw IMG_GetError();
  }

  try {
    load_image("./data/player1.png", "player1");
    load_image("./data/player2.png", "player2");
    load_image("./data/bg.png", "bg");
    load_image("./data/bg_red.png", "bg_red");
    load_image("./data/bg_green.png", "bg_green");
    load_image("./data/bg_blue.png", "bg_blue");
    load_image("./data/food.png", "food");
    load_image("./data/food_counter.png", "food_counter");
    load_image("./data/akabei.png", "akabei");
    load_image("./data/pinky.png", "pinky");
    load_image("./data/aosuke.png", "aosuke");
    load_image("./data/guzuta.png", "guzuta");
    load_image("./data/mon_run.png", "mon_run");
    load_image("./data/plate.png", "plate");
  } catch (const char &e) {
    throw e;
  }
}

SDL_Surface *Image_manager::get_image(const char *name) noexcept {
  Img_list *p = img_list_top_;
  while (p) {
    if (!strcmp(p->name, name)) {
      return p->img;
    }
    p = p->next;
  }

  return nullptr;
}

void Image_manager::delete_all_image() noexcept {
  while (img_list_top_) {
    Img_list *p = img_list_top_->next;
    SDL_FreeSurface(img_list_top_->img);
    delete img_list_top_;  // TODO: Correct code of freeing dynamic memory?
    img_list_top_ = p;
    if (img_list_top_) {
      img_list_top_->prev = nullptr;
    }
  }
  atexit(IMG_Quit);
}
