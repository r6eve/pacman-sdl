#include "image_manager.hpp"
#include <stdio.h>
#include <string.h>

static Img_list *Img_list_top = NULL;

void load_img(const char *path, const char *name) {
  SDL_Surface *img;
  if ((img = IMG_Load(path)) == NULL) {
    throw IMG_GetError();
  }

  Img_list *list;
  if ((list = new Img_list) == NULL) {
    SDL_FreeSurface(img);
    throw "out of memory";
  }
  strcpy(list->name, name);
  list->img = img;
  list->prev = NULL;
  list->next = Img_list_top;
  if (Img_list_top != NULL) {
    Img_list_top->prev = list;
  }
  Img_list_top = list;
}

SDL_Surface *get_img(const char *name) {
  Img_list *p = Img_list_top;
  while (p != NULL) {
    if (!strcmp(p->name, name)) {
      return p->img;
    }
    p = p->next;
  }

  return NULL;
}

void del_img(const char *name) {
  Img_list *p = Img_list_top;
  while (p != NULL) {
    if (!strcmp(p->name, name)) {
      if (p->next != NULL) {
        p->next->prev = p->prev;
      }
      if (p->prev != NULL) {
        p->prev->next = p->next;
      } else {
        Img_list_top = p->next;
      }
      SDL_FreeSurface(p->img);
      delete p;
      break;
    }
    p = p->next;
  }
}

void del_all_img() {
  while (Img_list_top != NULL) {
    Img_list *p = Img_list_top->next;
    SDL_FreeSurface(Img_list_top->img);
    delete Img_list_top;  // TODO: Correct code of freeing dynamic memory?
    Img_list_top = p;
    if (Img_list_top != NULL) {
      Img_list_top->prev = NULL;
    }
  }
}
