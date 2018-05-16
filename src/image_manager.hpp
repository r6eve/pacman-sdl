#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>
#include <memory>

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
  std::unique_ptr<SDL_Surface> image_array_[image::count];

  void load(const char *path, const unsigned int image_type);

 public:
  ImageManager() noexcept;

  SDL_Surface *get(const unsigned int image_type) const noexcept;

  ~ImageManager() noexcept;
};

#endif
