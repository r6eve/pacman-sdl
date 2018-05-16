#include <SDL/SDL_mixer.h>
#include <iostream>
#include <string.h>
#include "mixer_manager.hpp"

using namespace std;

MixerManager::MixerManager() noexcept {
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
    cerr << Mix_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  music_[0] = Mix_LoadMUS("./data/66376e_Pacman_Siren_Sound_Effect.mp3");
  music_[1] = Mix_LoadMUS("./data/pacman_beginning.wav");
  music_[2] = Mix_LoadMUS("./data/pacman_death.wav");
  se_ = Mix_LoadWAV("./data/pacman_chomp.wav");
  if (!music_[0] || !music_[1] || !music_[2] || !se_) {
    cerr << Mix_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
}

Mix_Music *MixerManager::get_music(const char *str) const noexcept {
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

Mix_Chunk *MixerManager::get_se(const char *str) const noexcept {
  if (!strcmp(str, "chomp")) {
    return se_;
  }
  return nullptr;
}

MixerManager::~MixerManager() noexcept {
  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  for (unsigned int i = 0; i < 3; ++i) {
    Mix_FreeMusic(music_[i]);
  }
  Mix_FreeChunk(se_);
  Mix_CloseAudio();
  atexit(Mix_Quit);
}
