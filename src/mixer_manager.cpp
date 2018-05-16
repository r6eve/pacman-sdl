#include <SDL/SDL_mixer.h>
#include <iostream>
#include <memory>
#include "mixer_manager.hpp"

using namespace std;

void MixerManager::load_music(const char *path, const unsigned char music_type) {
  music_[music_type] = Mix_LoadMUS(path);
  if (!music_[music_type]) {
    throw Mix_GetError();
  }
}

void MixerManager::load_se(const char *path) {
  const Mix_Chunk *se = Mix_LoadWAV(path);
  if (!se) {
    throw Mix_GetError();
  }
  se_ = make_unique<Mix_Chunk>(*se);
}

MixerManager::MixerManager() noexcept {
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
    cerr << Mix_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  try {
    load_music("./data/66376e_Pacman_Siren_Sound_Effect.mp3", music_type::siren);
    load_music("./data/pacman_beginning.wav", music_type::beginning);
    load_music("./data/pacman_death.wav", music_type::death);
    load_se("./data/pacman_chomp.wav");
  } catch (const char &e) {
    cerr << "error: " << e << '\n';
    exit(EXIT_FAILURE);
  }
}

Mix_Music *MixerManager::get_music(const unsigned char music_type) const noexcept {
  return music_[music_type];
}

Mix_Chunk *MixerManager::get_se() const noexcept {
  return se_.get();
}

MixerManager::~MixerManager() noexcept {
  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  for (unsigned int i = 0; i < music_type::count; ++i) {
    Mix_FreeMusic(music_[i]);
  }
  Mix_FreeChunk(se_.get());
  Mix_CloseAudio();
  atexit(Mix_Quit);
}
