#include <iostream>
#include <vector>
#include <fstream>

#include "render_window.h"
#include "render_tank0.h"
#include "main_controller.h"
#include "wsjcpp_core.h"

int main(int argc, char* args[]) {

    MainController *pMainController = new MainController("yourCityIsInvadedByAliens_Tomsk (v0.0.1)");

    if (!pMainController->initRenderWindow()) {
        return -1;
    }

    pMainController->loadGameDataWithProgressBar();

    SDL_Texture* pTextureBackground = pMainController->getWindow()->loadTexture("res/gfx/background.png");
    SDL_Texture* pTextureBuildingBlock = pMainController->getWindow()->loadTexture("res/gfx/building-block.png");
    SDL_Texture* pTextureAlienShip1 = pMainController->getWindow()->loadTexture("res/sprites/alien-ship.png");
    SDL_Texture* pTextureTank0 = pMainController->getWindow()->loadTexture("res/sprites/tank0.png");
    // SDL_Texture* pTextureRocket = pMainController->getWindow()->loadTexture("res/sprites/rocket.png");
    SDL_Texture* pTextureCursor = pMainController->getWindow()->loadTexture("res/gfx/mouse-target.png");

    CoordXY coordCenter = pMainController->getCoordCenter();
    
    // player
    pMainController->getWindow()->addObject(new RenderPlayerAlienShip1(coordCenter, pTextureAlienShip1, 1000));

    // text
    RenderAbsoluteTextBlock *pFpsText = new RenderAbsoluteTextBlock(CoordXY(10, 10), "FPS: ????", 1000);
    pMainController->getWindow()->addObject(pFpsText);

    // coordinates of player
    RenderAbsoluteTextBlock *pCoordText = new RenderAbsoluteTextBlock(CoordXY(10, 40), "x = ? y = ?", 1000);
    pMainController->getWindow()->addObject(pCoordText);

    GameTank0State *pTank0State = new GameTank0State(CoordXY(100,100));
    pMainController->getWindow()->addObject(new RenderTank0(
        pTank0State,
        pTextureTank0,
        1000
    ));

    bool gameRunning = true;

    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    pMainController->getGameState()->init();
    
    RenderMouse *pMouse = new RenderMouse(coordCenter, pTextureCursor, 2000);
    pMainController->getWindow()->addObject(pMouse);


    pMainController->getWindow()->sortObjectsByPositionZ();
    while (gameRunning) {
        pMainController->getGameState()->updateElapsedTime();
        pMainController->getWindow()->clear();
        pMainController->getWindow()->modifyObjects(*(pMainController->getGameState()));
        pMainController->getWindow()->drawObjects();

        SDL_Event event;
        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                std::cout << "SDL_KEYDOWN " << event.key.keysym.sym << std::endl;

                if (
                    !keyboard_state_array[SDL_SCANCODE_UP]
                    && !keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    pMainController->getGameState()->incrementCoordLeftTopY(-5);
                } else if (
                    keyboard_state_array[SDL_SCANCODE_UP]
                    && !keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    pMainController->getGameState()->incrementCoordLeftTopY(5);
                } else if (
                    !keyboard_state_array[SDL_SCANCODE_UP]
                    && keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    pMainController->getGameState()->incrementCoordLeftTopX(5);
                } else if (
                    !keyboard_state_array[SDL_SCANCODE_UP]
                    && !keyboard_state_array[SDL_SCANCODE_LEFT]
                    && keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    pMainController->getGameState()->incrementCoordLeftTopX(-5);
                } /*else if (
                    keyboard_state_array[SDL_SCANCODE_UP]
                    && keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    pMainController->getGameState()->incrementCoordLeftTopX(5);
                    pMainController->getGameState()->incrementCoordLeftTopY(5);
                }*/

                switch (event.key.keysym.sym) {
                    case SDLK_w: pMainController->getGameState()->incrementCoordLeftTopY(5); break;
                    case SDLK_s: pMainController->getGameState()->incrementCoordLeftTopY(-5); break;
                    case SDLK_a:  pMainController->getGameState()->incrementCoordLeftTopX(5); break;
                    case SDLK_d: pMainController->getGameState()->incrementCoordLeftTopX(-5); break;
                    case SDLK_ESCAPE:
                        if (pMainController->getGameState()->isMouseCaptured()) {
                            pMainController->getGameState()->setMouseCaptured(false);
                        }
                        break;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                if (pMainController->getGameState()->isMouseCaptured()) {
                    CoordXY p0(event.motion.x, event.motion.y);
                    pMouse->updateCoord(p0);
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (!pMainController->getGameState()->isMouseCaptured()) {
                    pMainController->getGameState()->setMouseCaptured(true);
                }
            }
        }

        // FPS
        nNumberOfFrames++;
        nElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - nStartTime;
        if (nElapsed > 3000) {
            double nFPS = nNumberOfFrames;
            nFPS /= nElapsed;
            nFPS *= 1000;
            pFpsText->updateText("FPS: ~" + std::to_string(int(nFPS)));
            std::cout << "FPS: ~" << int(nFPS) << std::endl;
            nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
            nNumberOfFrames = 0;
            std::string sCoordPlayer = "X=" + std::to_string(pMainController->getGameState()->getCoordLeftTop().x() + coordCenter.x())
                + " Y=" + std::to_string(pMainController->getGameState()->getCoordLeftTop().y() + coordCenter.y());
            pCoordText->updateText(sCoordPlayer);
        }
    }

    pMainController->getWindow()->cleanUp();
    SDL_Quit();
    return 0;
}