#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <wsjcpp_core.h>

#include "render_window.h"
#include "render_tank0.h"
#include "render_player_alient_ship.h"
#include "main_controller.h"
#include "utils_music_player.h"
#include "ykeyboard.h"

int main(int argc, char* args[]) {

    MainController *pMainController = new MainController("Your City Is Invaded Aliens (v0.0.1)");

    if (!pMainController->findResourceDir()) {
        return -1;
    }

    if (!pMainController->initRenderWindow()) {
        return -1;
    }

    if (!pMainController->loadGameDataWithProgressBar()) {
        return -1;
    }

    if (!pMainController->showStartDialog()) {
        return -1;
    }

    std::string sResourceDir = pMainController->getResourceDir();
    
    SDL_Texture* pTextureAlienShip1 = pMainController->getWindow()->loadTexture(sResourceDir + "/sprites/alien-ship.png");
    CoordXY coordCenter = pMainController->getCoordCenter();
    
    // player
    GameAlienShipState *pAlientShipState = pMainController->getGameAlienShipState();
    bool gameRunning = true;

    long nNumberOfFrames = 0;
    long nStartTime = getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    pMainController->getGameState()->init();
    pMainController->getWindow()->sortObjectsByPositionZ();
    pMainController->startAllThreads();

    UtilsMusicPlayer *pMusicPlayer = new UtilsMusicPlayer(
        sResourceDir,
        pMainController->getGameState()
    );
    pMusicPlayer->init();
    YKeyboard keyboard;

    while (gameRunning) {
        pMainController->getGameState()->updateElapsedTime();
        pMainController->getWindow()->clear();
        pMainController->modifyObjects();
        pMainController->getWindow()->drawObjects();

        SDL_Event event;
        keyboard.pollState();
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

        int nLeftPad = pMainController->getCoordCenter().x() + 320/2;
        int nRightPad = pMainController->getCoordCenter().x() - 320/2;
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