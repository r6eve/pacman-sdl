Pacman-SDL
==========
[![Build Status on Travis CI][]][Travis CI Results]

Pacman written in C++.

## Demo

![pacman-sdl-demo][]

## Installation

```console
# Arch
$ pacman -S sdl sdl_image sdl_mixer sdl_ttf
# Ubuntu
$ apt-get -y install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
# macOS
$ brew update
$ brew install sdl sdl_image sdl_mixer sdl_ttf
# If you use others, install those from package manager or source.

$ git clone git@github.com:r6eve/pacman-sdl.git
```

## Usage

```console
$ make
$ ./pacman-sdl
```

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
[pacman-sdl-demo]: https://raw.githubusercontent.com/r6eve/screenshots/master/pacman-sdl/pacman-sdl.gif
[SDL 1.2]: http://www.libsdl.org/
[SDL_image 1.2]: https://www.libsdl.org/projects/SDL_image/release-1.2.html
[SDL_mixer 1.2]: https://www.libsdl.org/projects/SDL_mixer/release-1.2.html
[SDL_ttf for SDL 1.2]: https://www.libsdl.org/projects/SDL_ttf/release-1.2.html
[源暎ゴシックP]: https://okoneya.jp/font/genei-antique.html
[Classic Gaming]: http://www.classicgaming.cc/classics/pac-man/sounds.php
[Sound FX Center]: http://soundfxcenter.com/download-sound/pacman-siren-sound-effect/
