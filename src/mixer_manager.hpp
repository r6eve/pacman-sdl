#ifndef MIXER_MANAGER_H
#define MIXER_MANAGER_H

#include <SDL/SDL_mixer.h>
#include <memory>

namespace music_type {
enum {
  siren = 0,
  beginning,
  death,
  count,
};
}  // namespace music_type

class MixerManager {
  Mix_Music *music_[music_type::count];
  std::unique_ptr<Mix_Chunk> se_;

  void load_music(const char *path, const unsigned char music_type);

  void load_se(const char *path);

 public:
  MixerManager() noexcept;

  Mix_Music *get_music(const unsigned char music_type) const noexcept;

  Mix_Chunk *get_se() const noexcept;

  ~MixerManager() noexcept;
};

#endif
