#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>

class Image_manager {
  typedef struct Img_list_rec {
    char name[64];
    SDL_Surface *img;
    struct Img_list_rec *prev;
    struct Img_list_rec *next;
  } Img_list;

  static Img_list *img_list_top_;

 public:
  static void load_image(const char *path, const char *name);
  static SDL_Surface *get_image(const char *name);
  static void delete_all_image();
};

#endif
