#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

namespace image {

enum {
  p1 = 0,
  p2,
  bg,
  bg_red,
  bg_green,
  bg_blue,
  food,
  food_counter,
  akabei,
  pinky,
  aosuke,
  guzuta,
  mon_run,
  plate,
  count,
};

}  // namespace image

class ImageManager {
  std::vector<SDL_Surface*> images_;

  // In macOS, using the following type makes `error: static_assert failed "the
  // specified hash does not meet the Hash requirements"`. Therefore, use an
  // array to manage images.
  //
  // `std::unordered_map<std::string, std::unique_ptr<SDL_Surface>> image_map_`
  inline void load(const char *path, const unsigned char image_type) {
    SDL_Surface *image = IMG_Load(path);
    if (!image) {
      throw IMG_GetError();
    }
    images_[image_type] = image;
  }

 public:
  ImageManager() noexcept {
    images_.reserve(image::count);

    const int flag = IMG_INIT_PNG;
    if ((IMG_Init(flag) & flag) != flag) {
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

  inline SDL_Surface *get(const unsigned char image_type) const noexcept {
    return images_[image_type];
  }

  ~ImageManager() noexcept { atexit(IMG_Quit); }
};

#endif
