#include <stdio.h>
#include <string.h>

#include "image_manager.hpp"

namespace image_manager {

namespace {

typedef struct Img_list_rec {
  char name[64];
  SDL_Surface *img;
  struct Img_list_rec *prev;
  struct Img_list_rec *next;
} Img_list;

static Img_list *Img_list_top = nullptr;

}  // namespace

void load_image(const char *path, const char *name) {
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
  list->next = Img_list_top;
  if (Img_list_top) {
    Img_list_top->prev = list;
  }
  Img_list_top = list;
}

SDL_Surface *get_image(const char *name) {
  Img_list *p = Img_list_top;
  while (p) {
    if (!strcmp(p->name, name)) {
      return p->img;
    }
    p = p->next;
  }

  return nullptr;
}

void delete_all_image() {
  while (Img_list_top) {
    Img_list *p = Img_list_top->next;
    SDL_FreeSurface(Img_list_top->img);
    delete Img_list_top;  // TODO: Correct code of freeing dynamic memory?
    Img_list_top = p;
    if (Img_list_top) {
      Img_list_top->prev = nullptr;
    }
  }
}

}  // namespace image_manager
