#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL_image.h>

typedef struct Img_list_rec {
  char name[64];
  SDL_Surface *img;
  struct Img_list_rec *prev;
  struct Img_list_rec *next;
} Img_list;

void load_img(const char *path, const char *name);
SDL_Surface *get_img(const char *name);
void del_img(const char *name);
void del_all_img();

#endif
