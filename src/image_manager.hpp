#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>

namespace image_manager {

void load_image(const char *path, const char *name);
SDL_Surface *get_image(const char *name);
void delete_all_image();

}  // namespace image_manager

#endif
