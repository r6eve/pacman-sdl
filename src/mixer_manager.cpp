#include <SDL/SDL_mixer.h>
#include <string.h>

#include "mixer_manager.hpp"

Mix_Music *Mixer_manager::music_[3] = {nullptr};
Mix_Chunk *Mixer_manager::se_ = nullptr;

void Mixer_manager::init() {
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
    throw Mix_GetError();
  }

  music_[0] = Mix_LoadMUS("./data/66376e_Pacman_Siren_Sound_Effect.mp3");
  music_[1] = Mix_LoadMUS("./data/pacman_beginning.wav");
  music_[2] = Mix_LoadMUS("./data/pacman_death.wav");
  se_ = Mix_LoadWAV("./data/pacman_chomp.wav");
  if (!music_[0] || !music_[1] || !music_[2] || !se_) {
    throw Mix_GetError();
  }
}

Mix_Music *Mixer_manager::get_music(const char *str) noexcept {
  if (!strcmp(str, "siren")) {
    return music_[0];
  }
  if (!strcmp(str, "beginning")) {
    return music_[1];
  }
  if (!strcmp(str, "death")) {
    return music_[2];
  }
  return nullptr;
}

Mix_Chunk *Mixer_manager::get_se(const char *str) noexcept {
  if (!strcmp(str, "chomp")) {
    return se_;
  }
  return nullptr;
}

void Mixer_manager::end() noexcept {
  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  for (unsigned int i = 0; i < 3; ++i) {
    Mix_FreeMusic(music_[i]);
  }
  Mix_FreeChunk(se_);
  Mix_CloseAudio();
  atexit(Mix_Quit);
}
