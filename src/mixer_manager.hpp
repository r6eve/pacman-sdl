#ifndef MIXER_MANAGER_H
#define MIXER_MANAGER_H

#include <SDL2/SDL_mixer.h>
#include <iostream>
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
  Mix_Music *music_list_[music_type::count];
  std::unique_ptr<Mix_Chunk> se_;

  inline void load_music(const char *path, const unsigned char music_type) noexcept {
    music_list_[music_type] = Mix_LoadMUS(path);
    if (!music_list_[music_type]) {
      std::cerr << "error: " << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
  }

  inline void load_se(const char *path) noexcept {
    const Mix_Chunk *se = Mix_LoadWAV(path);
    if (!se) {
      std::cerr << "error: " << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    se_ = std::make_unique<Mix_Chunk>(*se);
  }

 public:
  MixerManager() noexcept {
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) !=
        0) {
      std::cerr << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    load_music("./data/66376e_Pacman_Siren_Sound_Effect.mp3",
               music_type::siren);
    load_music("./data/pacman_beginning.wav", music_type::beginning);
    load_music("./data/pacman_death.wav", music_type::death);
    load_se("./data/pacman_chomp.wav");
  }

  inline Mix_Music *get_music(const unsigned char music_type) const noexcept {
    return music_list_[music_type];
  }

  inline Mix_Chunk *get_se() const noexcept { return se_.get(); }

  ~MixerManager() noexcept {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    for (const auto &music : music_list_) {
      Mix_FreeMusic(music);
    }
    Mix_FreeChunk(se_.get());
    Mix_CloseAudio();
    atexit(Mix_Quit);
  }
};

#endif
