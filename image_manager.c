#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "image_manager.h"

static Img_list *Img_list_top = NULL;

int load_img(const char *path, const char *name) {
  SDL_Surface *img = IMG_Load(path);
  if (img == NULL) {
    fprintf(stderr, "cannot open [%s]: %s.\n", path, IMG_GetError());
    return 0;
  }

  Img_list *new;
  if ((new = malloc(sizeof(Img_list))) == NULL) {
    fprintf(stderr, "[%s]: out of memory.\n", path);
    SDL_FreeSurface(img);
    return 0;
  }
  strcpy(new->name, name);
  new->img  = img;
  new->prev = NULL;
  new->next = Img_list_top;
  if (Img_list_top != NULL) {
    Img_list_top->prev = new;
  }
  Img_list_top = new;
  return 1;
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
      free(p);
      break;
    }
    p = p->next;
  }
}

void del_all_img() {
  while (Img_list_top != NULL) {
    Img_list *p = Img_list_top->next;
    SDL_FreeSurface(Img_list_top->img);
    free(Img_list_top); // TODO: Correct code of freeing dynamic memory?
    Img_list_top = p;
    if (Img_list_top != NULL) {
      Img_list_top->prev = NULL;
    }
  }
}
