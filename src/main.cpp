#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "render_window.h"
#include "Entity.hpp"

int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
    }

    RenderWindow window("yourCityIsInvadedByAliens_Tomsk (v0.0.1)", 1280, 720);

    SDL_Texture* grassTexture = window.loadTexture("res/gfx/ground_grass_1.png");

    std::vector<Entity> entitiees = {
        Entity(Vector2f(0, 0), grassTexture),
        Entity(Vector2f(30, 0), grassTexture),
        Entity(Vector2f(30, 30), grassTexture),
        Entity(Vector2f(30, 60), grassTexture),
        Entity(Vector2f(100, 100), grassTexture)
    };

    {
        Entity wilson(Vector2f(100, 50), grassTexture);
        entitiees.push_back(wilson);
    }

    bool gameRunning = true;

    SDL_Event event;

    int nNumberOfFrames = 0;
    
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
        window.display();
    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}