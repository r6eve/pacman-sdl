#include <SDL/SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "image_manager.hpp"

Image_manager::Img_list *Image_manager::img_list_top_ = nullptr;

void Image_manager::load_image(const char *path, const char *name) {
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
  list->next = img_list_top_;
  if (img_list_top_) {
    img_list_top_->prev = list;
  }
  img_list_top_ = list;
}

SDL_Surface *Image_manager::get_image(const char *name) {
  Img_list *p = img_list_top_;
  while (p) {
    if (!strcmp(p->name, name)) {
      return p->img;
    }
    p = p->next;
  }

  return nullptr;
}

void Image_manager::delete_all_image() {
  while (img_list_top_) {
    Img_list *p = img_list_top_->next;
    SDL_FreeSurface(img_list_top_->img);
    delete img_list_top_;  // TODO: Correct code of freeing dynamic memory?
    img_list_top_ = p;
    if (img_list_top_) {
      img_list_top_->prev = nullptr;
    }
  }
}
