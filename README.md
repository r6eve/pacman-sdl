Pacman-SDL
==========
[![Build Status on GitHub Actions][]][GitHub Actions Results]
[![Build Status on AppVeyor][]][AppVeyor Results]

Pacman written in C++.

## Demo Video (Clickable)

[![Demo Image][]][Demo Video]

## Installation

```console
# Arch
> pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf
# Ubuntu
> apt-get -y install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

# macOS
> brew update
> brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf

# Windows
# Cf. .appveyor.yml

# If you use others, install those from package manager or source.

> git clone git@github.com:r6eve/pacman-sdl.git  # Or download ZIP archive and extract pacman-sdl.
> cd pacman-sdl
> make
```

## Usage

```console
> ./pacman-sdl --help
Usage: pacman-sdl [options]

Options:
    -d  --debug         debug mode
    -h, --help          print this help menu
    -v, --version       print version
> ./pacman-sdl  # normal mode
> ./pacman-sdl -d  # debug mode
```

### Key Config

* Player1
  * UP, LEFT, DOWN, RIGHT: ↑ ← ↓ →
* Player2
  * UP, LEFT, DOWN, RIGHT: W A S D

## Requirement

* g++ / clang++
* [SDL 2.0][]
* [SDL_image 2.0][]
* [SDL_mixer 2.0][]
* [SDL_ttf 2.0][]

## Used Resources

* [源暎ゴシックP][]
* [Ramine / music][]
* [Classic Gaming][]
* [Sound FX Center][]

## TODO

* Record Score

[Build Status on GitHub Actions]: https://github.com/r6eve/pacman-sdl/workflows/main/badge.svg
[GitHub Actions Results]: https://github.com/r6eve/pacman-sdl/actions
[Build Status on AppVeyor]: https://ci.appveyor.com/api/projects/status/r0owflvuqcyl651d?svg=true
[AppVeyor Results]: https://ci.appveyor.com/project/r6eve/pacman-sdl
[Demo Image]: http://img.youtube.com/vi/jTj5H4HFE68/0.jpg
[Demo Video]: https://youtu.be/jTj5H4HFE68
[SDL 2.0]: http://www.libsdl.org/
[SDL_image 2.0]: https://www.libsdl.org/projects/SDL_image/
[SDL_mixer 2.0]: https://www.libsdl.org/projects/SDL_mixer/
[SDL_ttf 2.0]: https://www.libsdl.org/projects/SDL_ttf/
[源暎ゴシックP]: https://okoneya.jp/font/genei-antique.html
[Ramine / music]: http://2ram.com/music.php
[Classic Gaming]: http://www.classicgaming.cc/classics/pac-man/sounds.php
[Sound FX Center]: http://soundfxcenter.com/download-sound/pacman-siren-sound-effect/
