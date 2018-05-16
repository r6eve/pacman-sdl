#include <SDL/SDL_image.h>
#include <string.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "image_manager.hpp"

using namespace std;

void ImageManager::load(const char *path, const char *name) {
  SDL_Surface *image = IMG_Load(path);
  if (!image) {
    throw IMG_GetError();
  }
  image_map_[name] = make_unique<SDL_Surface>(*image);
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
  unordered_map<string, unique_ptr<SDL_Surface>>::const_iterator got =
      image_map_.find(name);
  if (got == image_map_.end()) {
    return nullptr;
  } else {
    return got->second.get();
  }
}

ImageManager::~ImageManager() noexcept { atexit(IMG_Quit); }
