#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <wsjcpp_core.h>

#include "render_window.h"
#include "transports/render_tank0.h"
#include "render_alienship.h"
#include "main_controller.h"
#include "ykeyboard.h"

int main(int argc, char* args[]) {

    MainController *pMainController = new MainController("Your City Is Invaded Aliens (v0.0.1)");

    if (!pMainController->init()) {
        return -1;
    }

    YKeyboard keyboard;
    CoordXY coordCenter = pMainController->getCoordCenter();
    GameAlienShipState *pAlientShipState = pMainController->getGameState()->getAlienShipState();

    if (!pMainController->loadGameDataWithProgressBar()) {
        return -1;
    }

    // if (!pMainController->showStartDialog()) {
    //     return -1;
    // }

    // player
    pMainController->getWindow()->sortObjectsByPositionZ();
    pMainController->startAllThreads();

    bool gameRunning = true;
    pMainController->startFpsCounting();
    while (gameRunning) {
        // TODO pMainController->getGameState()->isShowLoader();

        pMainController->getGameState()->updateElapsedTime();
        pMainController->clearWindow();
        pMainController->modifyObjects();
        pMainController->drawObjects();

        SDL_Event event;
        keyboard.pollState();
        pMainController->getSoundController()->update();

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
                        // if (pMainController->getGameState()->isMouseCaptured()) {
                        //     pMainController->getGameState()->setMouseCaptured(false);
                        // }
                    }
                }

                if (keyboard.isF12()) {
                    pMainController->toggleFullscreen();
                }

                if (keyboard.isF1()) {
                    // pMainController->getWindow()->toggleFullscreen();
                    // TODO show help and pause of the game
                }

                if (keyboard.isSpace()) {
                    pAlientShipState->setShooting(true);
                } else {
                    pAlientShipState->setShooting(false);
                }

                if (keyboard.isUp()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::UP);
                } else if (keyboard.isUpLeft()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::UP_LEFT);
                } else if (keyboard.isUpRight()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::UP_RIGHT);
                } else if (keyboard.isDown()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::DOWN);
                } else if (keyboard.isDownLeft()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::DOWN_LEFT);
                } else if (keyboard.isDownRight()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::DOWN_RIGHT);
                } else if (keyboard.isLeft()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::LEFT);
                } else if (keyboard.isRight()) {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::RIGHT);
                } else {
                    pAlientShipState->setMoveDirection(MoveObjectDirection::NONE);
                }
            }
        }

        int nLeftPad = pMainController->getCoordCenter().x();
        int nRightPad = pMainController->getCoordCenter().x() - 320;
        int nTopPad = pMainController->getCoordCenter().y();
        int nBottomPad = pMainController->getCoordCenter().y();
        pAlientShipState->move(
            pMainController->getGameState()->getElapsedTime(),
            pMainController->getGameState()->getMinPoint(),
            pMainController->getGameState()->getMaxPoint(),
            nLeftPad,
            nRightPad,
            nTopPad,
            nBottomPad
        );

        // window must move to the player
        /*
            360 - w/2 - 320/2
        */
        CoordXY newLeftTop = pAlientShipState->getPosition() - pMainController->getCoordCenter() + CoordXY(320/2, 0);
        pMainController->getGameState()->setCoordLeftTop(newLeftTop);

        // FPS
        pMainController->updateFps();
        pMainController->updatePlayerCoord();
    }

    pMainController->getWindow()->cleanUp();
    // Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}