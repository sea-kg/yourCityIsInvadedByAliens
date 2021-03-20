#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

#include "render_window.h"
#include "render_tank0.h"
#include "render_player_alient_ship.h"
#include "main_controller.h"

int main(int argc, char* args[]) {

    MainController *pMainController = new MainController("yourCityIsInvadedByAliens_Tomsk (v0.0.1)");

    if (!pMainController->initRenderWindow()) {
        return -1;
    }

    if (!pMainController->loadGameDataWithProgressBar()) {
        return -1;
    }

    SDL_Texture* pTextureBackground = pMainController->getWindow()->loadTexture("res/gfx/background.png");
    SDL_Texture* pTextureBuildingBlock = pMainController->getWindow()->loadTexture("res/gfx/building-block.png");
    SDL_Texture* pTextureAlienShip1 = pMainController->getWindow()->loadTexture("res/sprites/alien-ship.png");
    SDL_Texture* pTextureCursor = pMainController->getWindow()->loadTexture("res/gfx/mouse-target.png");

    CoordXY coordCenter = pMainController->getCoordCenter();
    
    // player
    GameAlienShipState *pAlientShipState = pMainController->getGameAlienShipState();
    bool gameRunning = true;

    long nNumberOfFrames = 0;
    long nStartTime = getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    pMainController->getGameState()->init();
    
    // RenderMouse *pMouse = new RenderMouse(coordCenter, pTextureCursor, 2000);
    // pMainController->getWindow()->addObject(pMouse);

    pMainController->getWindow()->sortObjectsByPositionZ();
    pMainController->getGameState()->setMouseCaptured(false);
    while (gameRunning) {
        pMainController->getGameState()->updateElapsedTime();
        pMainController->getWindow()->clear();
        pMainController->modifyObjects();
        pMainController->getWindow()->drawObjects();

        SDL_Event event;
        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                std::cout << "SDL_KEYDOWN " << event.key.keysym.sym << std::endl;
            }

            if (event.type == SDL_KEYUP) {
                std::cout << "SDL_KEYUP " << event.key.keysym.sym << std::endl;
            }

            if (event.type == SDL_QUIT) {
                gameRunning = false;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {

                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    if (pMainController->isFullscreen()) {
                        pMainController->toggleFullscreen();
                    } else {
                        if (pMainController->getGameState()->isMouseCaptured()) {
                            pMainController->getGameState()->setMouseCaptured(false);
                        }
                    }
                }

                if (pMainController->isKeyboardF12(keyboard_state_array)) {
                    pMainController->toggleFullscreen();
                }

                if (pMainController->isKeyboardF1(keyboard_state_array)) {
                    // pMainController->getWindow()->toggleFullscreen();
                    // TODO show help and pause of the game
                }

                if (pMainController->isKeyboardSpace(keyboard_state_array)) {
                    pAlientShipState->setShooting(true);
                } else {
                    pAlientShipState->setShooting(false);
                }

                if (pMainController->isKeyboardUp(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::UP);
                } else if (pMainController->isKeyboardUpLeft(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::UP_LEFT);
                } else if (pMainController->isKeyboardUpRight(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::UP_RIGHT);
                } else if (pMainController->isKeyboardDown(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::DOWN);
                } else if (pMainController->isKeyboardDownLeft(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::DOWN_LEFT);
                } else if (pMainController->isKeyboardDownRight(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::DOWN_RIGHT);
                } else if (pMainController->isKeyboardLeft(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::LEFT);
                } else if (pMainController->isKeyboardRight(keyboard_state_array)) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::RIGHT);
                } else {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::NONE);
                }

            } // else if (event.type == SDL_MOUSEMOTION) {
              //   if (pMainController->getGameState()->isMouseCaptured()) {
              //       CoordXY p0(event.motion.x, event.motion.y);
              //       pMouse->updateCoord(p0);
              //   }
            // } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            //     if (!pMainController->getGameState()->isMouseCaptured()) {
            //         pMainController->getGameState()->setMouseCaptured(true);
            //     }
            // }
        }
        pAlientShipState->move(pMainController->getGameState()->getElapsedTime());

        // window must move to the player
        /*
        0
            360 - w/2 - 320/2
        */
        // std::cout << "------" << std::endl;
        // std::cout << pMainController->getCoordCenter().x() << std::endl;
        // std::cout << pMainController->getCoordCenter().y() << std::endl;
        // std::cout << pAlientShipState->getPosition().x() << std::endl;
        // std::cout << pAlientShipState->getPosition().y() << std::endl;

        CoordXY newLeftTop = pAlientShipState->getPosition() - pMainController->getCoordCenter() + CoordXY(320/2, 0);
        // std::cout << newLeftTop.x() << std::endl;
        // std::cout << newLeftTop.y() << std::endl;

        pMainController->getGameState()->setCoordLeftTop(newLeftTop);

        // FPS
        nNumberOfFrames++;
        nElapsed = getCurrentTimeInMilliseconds() - nStartTime;
        if (nElapsed > 3000) {
            double nFPS = nNumberOfFrames;
            nFPS /= nElapsed;
            nFPS *= 1000;
            pMainController->updateFpsValue(nFPS);
            nStartTime = getCurrentTimeInMilliseconds();
            nNumberOfFrames = 0;
        }

        if (nElapsed > 500) {
            pMainController->updatePlayerCoord();
        }
    }

    pMainController->getWindow()->cleanUp();
    // Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}