# Game: Your City Is Invaded By Aliens

[![Build Status](https://travis-ci.com/sea-kg/yourCityIsInvadedByAliens.svg?branch=main)](https://travis-ci.com/github/sea-kg/yourCityIsInvadedByAliens) [![Github Stars](https://img.shields.io/github/stars/sea-kg/yourCityIsInvadedByAliens.svg?label=github%20%E2%98%85)](https://github.com/sea-kg/yourCityIsInvadedByAliens/) [![Github Stars](https://img.shields.io/github/contributors/sea-kg/yourCityIsInvadedByAliens.svg)](https://github.com/sea-kg/yourCityIsInvadedByAliens/) [![Github Forks](https://img.shields.io/github/forks/sea-kg/yourCityIsInvadedByAliens.svg?label=github%20forks)](https://github.com/sea-kg/yourCityIsInvadedByAliens/) [![Total alerts](https://img.shields.io/lgtm/alerts/g/sea-kg/yourCityIsInvadedByAliens.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/sea-kg/yourCityIsInvadedByAliens/alerts/) [![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/sea-kg/yourCityIsInvadedByAliens.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/sea-kg/yourCityIsInvadedByAliens/context:python) [![Language grade: Python](https://img.shields.io/lgtm/grade/cpp/g/sea-kg/yourCityIsInvadedByAliens.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/sea-kg/yourCityIsInvadedByAliens/context:cpp)

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
$ sudo apt install libsdl2-mixer-dev
```

### MacOS 

```
% brew install cmake
% brew install sdl2
% brew install sdl2_image
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

## Target controls

- keyboard *for desctops* - **Done**
- ~~joystick *for desctops*~~ - **Yet not**
- ~~touchscreen *for mobiles*~~ - **Yet not**

## Target platforms

- linux (Ubuntu 20.04) - **Compiled**
- windows - **Need check**
- mac - **Need check again**
- ~~android~~ - **Not configured yet**
- ~~ios~~ - **Not configured yet**
- ~~apple tv~~ - **Not configured yet**

## Structure of directory `res`

- `./res/app` - contains basic configuration / textures for panels, buttons and etc...
- `./res/default` - contains default map / textures / sprites and etc
- `./res/asset-factories-bootscreen` - contains all resources needed for bootscreen
- `./res/asset-factories` - contains all resources needed for game


## Other instruments

- cmake - for compiling project
- Visual Studio Code - for editing code 
- Gimp - for design sprites/textures and etc.

## useful links

* [SDL_Keycode] (https://wiki.libsdl.org/SDL_Keycode)
* https://www.youtube.com/watch?v=WK_ojz7TOdM
* https://www.youtube.com/watch?v=OrkpfmXHP_Q
* https://www.youtube.com/watch?v=h-a4KAWuMT0
* https://tatrix.org/sdl-tutorial/11/

## Interesting projects

* https://docs.scummvm.org/

