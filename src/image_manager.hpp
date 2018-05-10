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

  static void load_image(const char *path, const char *name);

 public:
  /**
   * Call this function before using get_image() .
   */
  static void init_image();

  static SDL_Surface *get_image(const char *name) noexcept;

  /**
   * Call this function when program exits.
   */
  static void delete_all_image() noexcept;
};

#endif
