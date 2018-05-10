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

  static void load(const char *path, const char *name);

 public:
  /**
   * Call this function before using get() .
   */
  static void init();

  static SDL_Surface *get(const char *name) noexcept;

  /**
   * Call this function when program exits.
   */
  static void end() noexcept;
};

#endif
