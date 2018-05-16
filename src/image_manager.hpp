#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>
#include <memory>
#include <unordered_map>

class ImageManager {
  std::unordered_map<std::string, std::unique_ptr<SDL_Surface>> image_map_;

  void load(const char *path, const char *name);

 public:
  ImageManager() noexcept;

  SDL_Surface *get(const char *name) const noexcept;

  ~ImageManager() noexcept;
};

#endif
