#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>

namespace image_manager {

// TODO: priavate struct/class
typedef struct Img_list_rec {
  char name[64];
  SDL_Surface *img;
  struct Img_list_rec *prev;
  struct Img_list_rec *next;
} Img_list;

void load_image(const char *path, const char *name);
SDL_Surface *get_image(const char *name);
void delete_all_image();

}  // namespace image_manager

#endif
