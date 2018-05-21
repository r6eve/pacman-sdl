Pacman-SDL
==========
[![Build Status on Travis CI][]][Travis CI Results]
[![Build Status on AppVeyor][]][AppVeyor Results]

Pacman written in C++.

## Demo Video

[![Demo Image][]][Demo Video]

## Installation

```console
# Arch
> pacman -S sdl sdl_image sdl_mixer sdl_ttf
# Ubuntu
> apt-get -y install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev

# macOS
> brew update
> brew install sdl sdl_image sdl_mixer sdl_ttf

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
  * UP: Up Arrow, LEFT: Left Arrow, DOWN: Down Arrow, RIGHT: Right Arrow
* Player2
  * UP: W, LEFT: A, DOWN: S, RIGHT: D

## Requirement

* g++ / clang++
* [SDL 1.2][]
* [SDL_image 1.2][]
* [SDL_mixer 1.2][]
* [SDL_ttf for SDL 1.2][]

## Used Resources

* [源暎ゴシックP][]
* [Classic Gaming][]
* [Sound FX Center][]

## TODO

* Support Version 2 of SDL
* Record Score

[Build Status on Travis CI]: https://travis-ci.org/r6eve/pacman-sdl.svg?branch=master
[Travis CI Results]: https://travis-ci.org/r6eve/pacman-sdl
[Build Status on AppVeyor]: https://ci.appveyor.com/api/projects/status/r0owflvuqcyl651d?svg=true
[AppVeyor Results]: https://ci.appveyor.com/project/r6eve/pacman-sdl
[Demo Image]: http://img.youtube.com/vi/jTj5H4HFE68/0.jpg
[Demo Video]: https://youtu.be/jTj5H4HFE68
[SDL 1.2]: http://www.libsdl.org/
[SDL_image 1.2]: https://www.libsdl.org/projects/SDL_image/release-1.2.html
[SDL_mixer 1.2]: https://www.libsdl.org/projects/SDL_mixer/release-1.2.html
[SDL_ttf for SDL 1.2]: https://www.libsdl.org/projects/SDL_ttf/release-1.2.html
[源暎ゴシックP]: https://okoneya.jp/font/genei-antique.html
[Classic Gaming]: http://www.classicgaming.cc/classics/pac-man/sounds.php
[Sound FX Center]: http://soundfxcenter.com/download-sound/pacman-siren-sound-effect/
