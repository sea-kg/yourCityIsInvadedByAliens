#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "render_window.h"
#include "Entity.hpp"
#include "wsjcpp_core.h"

int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
    }
    RenderStateObjects stateObjects;

    RenderWindow window("yourCityIsInvadedByAliens_Tomsk (v0.0.1)", 1280, 720);

    SDL_Texture* grassTexture = window.loadTexture("res/gfx/ground_grass_1.png");

    std::vector<Entity> entitiees = {
        Entity(CoordXY(0, 0), grassTexture),
        Entity(CoordXY(30, 0), grassTexture),
        Entity(CoordXY(30, 30), grassTexture),
        Entity(CoordXY(30, 60), grassTexture),
        Entity(CoordXY(100, 100), grassTexture)
    };

    {
        Entity wilson(CoordXY(100, 50), grassTexture);
        entitiees.push_back(wilson);
    }

    // triangle
    CoordXY p0(320, 200);
    CoordXY p1(300, 240);
    CoordXY p2(340, 240);
    window.addObject(new RenderLine(p0, p1));
    window.addObject(new RenderLine(p1, p2));
    window.addObject(new RenderLine(p2, p0));

    window.sortObjectsByPositionZ();

    bool gameRunning = true;

    SDL_Event event;

    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    while (gameRunning) {

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }

        window.clear();
        for (Entity& e : entitiees) {
            window.render(e);
        }
        window.drawObjects();

        // FPS
        nNumberOfFrames++;
        nElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - nStartTime;
        if (nElapsed > 3000) {
            double nFPS = nNumberOfFrames;
            nFPS /= nElapsed;
            nFPS *= 1000;
            std::cout << "FPS: " << nFPS << std::endl;
            nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
            nNumberOfFrames = 0;
        }
    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}