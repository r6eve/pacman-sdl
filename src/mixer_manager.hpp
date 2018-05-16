#ifndef MIXER_MANAGER_H
#define MIXER_MANAGER_H

#include <SDL/SDL_mixer.h>

class MixerManager {
  Mix_Music *music_[3];  // TODO: enum class
  Mix_Chunk *se_;

 public:
  MixerManager() noexcept;

  Mix_Music *get_music(const char *str) const noexcept;

  Mix_Chunk *get_se(const char *str) const noexcept;

  ~MixerManager() noexcept;
};

#endif
