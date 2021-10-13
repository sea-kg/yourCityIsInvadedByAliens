# Game: Your City Is Invaded By Aliens

[![Build Status](https://travis-ci.com/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg?branch=main)](https://travis-ci.com/sea-kg/yourCityIsInvadedByAliens_Tomsk) [![Github Stars](https://img.shields.io/github/stars/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg?label=github%20%E2%98%85)](https://github.com/sea-kg/yourCityIsInvadedByAliens_Tomsk/) [![Github Stars](https://img.shields.io/github/contributors/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg)](https://github.com/sea-kg/yourCityIsInvadedByAliens_Tomsk/) [![Github Forks](https://img.shields.io/github/forks/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg?label=github%20forks)](https://github.com/sea-kg/yourCityIsInvadedByAliens_Tomsk/) [![Total alerts](https://img.shields.io/lgtm/alerts/g/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/sea-kg/yourCityIsInvadedByAliens_Tomsk/alerts/) [![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/sea-kg/yourCityIsInvadedByAliens_Tomsk/context:python) [![Language grade: Python](https://img.shields.io/lgtm/grade/cpp/g/sea-kg/yourCityIsInvadedByAliens_Tomsk.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/sea-kg/yourCityIsInvadedByAliens_Tomsk/context:cpp) [![deepcode](https://www.deepcode.ai/api/gh/badge?key=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwbGF0Zm9ybTEiOiJnaCIsIm93bmVyMSI6InNlYS1rZyIsInJlcG8xIjoieW91ckNpdHlJc0ludmFkZWRCeUFsaWVuc19Ub21zayIsImluY2x1ZGVMaW50IjpmYWxzZSwiYXV0aG9ySWQiOjE1NjQxLCJpYXQiOjE2MTgwNTk1Nzh9.TepnsVXpVZe3MscukV6MJFJP7yJT2CUh36MR9rM3bwA)](https://www.deepcode.ai/app/gh/sea-kg/yourCityIsInvadedByAliens_Tomsk/_/dashboard?utm_content=gh%2Fsea-kg%2FyourCityIsInvadedByAliens_Tomsk)

Game 2D with open source code based on cross platform graphical library SDL2.

[blog in vk](https://vk.com/yourcityisinvadedbyaliens)

![Alt text](/contrib/main-screen.jpg?raw=true "Main Screen")

## Installing

TODO

## Building

### Ubuntu

```
$ sudo apt install cmake g++
$ sudo apt install libsdl2-dev
$ sudo apt install libsdl2-image-dev
$ sudo apt install libsdl2-ttf-dev
$ sudo apt install libsdl2-mixer-dev
```

### MacOS 

```
% brew install cmake
% brew install sdl2
% brew install sdl2_image
% brew install sdl2_ttf
% brew install sdl2_mixer
```

Build:
```
% ./build_simple.sh
```

Run:
```
% ./yourCityIsInvadedByAliens
```

### Windows  (msvc2015 x86):

Requirements:
- Visual Studio 2015 (x86)
- CMake
- powershell

Just run a script for download SDL2 libraries and build project:

```
> msvc2015x86_build_simple.bat
```

## Struct of res

`./res/app` - contains basic configuration / textures for panels, buttons and etc...
`./res/default` - contains default map / textures / sprites and etc
`./res/buildings` - contains all type of buildings


## Other Intruments

- cmake - for compiling project
- Visual Studio Code - for editing code 
- Gimp - for design sprites/textures and etc.

## useful links

* [SDL_Keycode] (https://wiki.libsdl.org/SDL_Keycode)
* https://www.youtube.com/watch?v=WK_ojz7TOdM
* https://www.youtube.com/watch?v=OrkpfmXHP_Q
* https://www.youtube.com/watch?v=h-a4KAWuMT0
* https://tatrix.org/sdl-tutorial/11/
