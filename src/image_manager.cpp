#include "image_manager.hpp"
#include <SDL/SDL_image.h>
#include <string.h>
#include <iostream>
#include <memory>

// In macOS, using the following type makes `error: static_assert failed "the
// specified hash does not meet the Hash requirements"`. Therefore, use an array
// to manage images.
//
// `std::unordered_map<std::string, std::unique_ptr<SDL_Surface>> image_map_`
void ImageManager::load(const char *path, const unsigned int image_type) {
  SDL_Surface *image = IMG_Load(path);
  if (!image) {
    throw IMG_GetError();
  }
  image_array_[image_type] = std::make_unique<SDL_Surface>(*image);
}

ImageManager::ImageManager() noexcept {
  const int flag = IMG_INIT_PNG;
  if (IMG_Init(flag) != flag) {
    std::cerr << "error: " << IMG_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  try {
    load("./data/player1.png", image::p1);
    load("./data/player2.png", image::p2);
    load("./data/bg.png", image::bg);
    load("./data/bg_red.png", image::bg_red);
    load("./data/bg_green.png", image::bg_green);
    load("./data/bg_blue.png", image::bg_blue);
    load("./data/food.png", image::food);
    load("./data/food_counter.png", image::food_counter);
    load("./data/akabei.png", image::akabei);
    load("./data/pinky.png", image::pinky);
    load("./data/aosuke.png", image::aosuke);
    load("./data/guzuta.png", image::guzuta);
    load("./data/mon_run.png", image::mon_run);
    load("./data/plate.png", image::plate);
  } catch (const char &e) {
    std::cerr << "error: " << e << '\n';
    exit(EXIT_FAILURE);
  }
}

SDL_Surface *ImageManager::get(const unsigned int image_type) const noexcept {
  return image_array_[image_type].get();
}

ImageManager::~ImageManager() noexcept { atexit(IMG_Quit); }
