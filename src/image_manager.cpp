#include <SDL/SDL_image.h>
#include <iostream>
#include <string.h>
#include "image_manager.hpp"

using namespace std;

void ImageManager::load(const char *path, const char *name) {
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

ImageManager::ImageManager() noexcept {
  const int flag = IMG_INIT_PNG;
  if (IMG_Init(flag) != flag) {
    cerr << "error: " << IMG_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  try {
    load("./data/player1.png", "p1");
    load("./data/player2.png", "p2");
    load("./data/bg.png", "bg");
    load("./data/bg_red.png", "bg_red");
    load("./data/bg_green.png", "bg_green");
    load("./data/bg_blue.png", "bg_blue");
    load("./data/food.png", "food");
    load("./data/food_counter.png", "food_counter");
    load("./data/akabei.png", "akabei");
    load("./data/pinky.png", "pinky");
    load("./data/aosuke.png", "aosuke");
    load("./data/guzuta.png", "guzuta");
    load("./data/mon_run.png", "mon_run");
    load("./data/plate.png", "plate");
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    exit(EXIT_FAILURE);
  }
}

SDL_Surface *ImageManager::get(const char *name) const noexcept {
  Img_list *p = img_list_top_;
  while (p) {
    if (!strcmp(p->name, name)) {
      return p->img;
    }
    p = p->next;
  }

  return nullptr;
}

ImageManager::~ImageManager() noexcept {
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
