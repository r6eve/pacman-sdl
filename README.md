Pacman-SDL
==========
[![Build Status][]][CI Results]

Pacman written in C++.

## Demo

![pacman-sdl-demo][pacman-sdl-demo]

## Installation

```console
# Arch
$ pacman -S sdl sdl_image sdl_mixer
# Ubuntu
$ apt-get -y install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev
# macOS
$ brew update
$ brew install sdl sdl_image sdl_mixer
# If you use others, install those from package manager or source.

$ git clone git@github.com:r6eve/pacman-sdl.git
```

## Usage

```console
$ make
$ ./pacman-sdl
```

## Requirement

* g++
* [SDL(Ver. 1)][sdl_ver_1]

Some libraries are downloaded from
* [SDL_kanji][sdl_kanji]

Sound are downloaded from
* [Classic Gaming][classic_gaming]
* [Sound FX Center][sound_fx_center]

## TODO

* Support Version 2 of SDL
* Record Score

[Build Status]: https://travis-ci.org/r6eve/pacman-sdl.svg?branch=master
[CI Results]: https://travis-ci.org/r6eve/pacman-sdl
[pacman-sdl-demo]: https://raw.githubusercontent.com/r6eve/screenshots/master/pacman-sdl/pacman-sdl.gif
[sdl_ver_1]: http://www.libsdl.org/
[sdl_kanji]: http://shinh.skr.jp/sdlkanji/
[classic_gaming]: http://www.classicgaming.cc/classics/pac-man/sounds.php
[sound_fx_center]: http://soundfxcenter.com/download-sound/pacman-siren-sound-effect/
