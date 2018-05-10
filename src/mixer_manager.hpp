#ifndef MIXER_MANAGER_H
#define MIXER_MANAGER_H

#include <SDL/SDL_mixer.h>

class Mixer_manager {
  static Mix_Music *music_[3];  // TODO: enum class
  static Mix_Chunk *se_;

 public:
  /**
   * Call this function before using get_music() or get_se() .
   */
  static void init();

  static Mix_Music *get_music(const char *str) noexcept;

  static Mix_Chunk *get_se(const char *str) noexcept;

  /**
   * Call this function when program exits.
   */
  static void end() noexcept;
};

#endif
