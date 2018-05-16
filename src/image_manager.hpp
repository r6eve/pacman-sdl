#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>

class ImageManager {
  typedef struct Img_list_rec {
    char name[64];
    SDL_Surface *img;
    struct Img_list_rec *prev;
    struct Img_list_rec *next;
  } Img_list;

  Img_list *img_list_top_;

  void load(const char *path, const char *name);

 public:
  ImageManager() noexcept;

  SDL_Surface *get(const char *name) const noexcept;

  ~ImageManager() noexcept;
};

#endif
