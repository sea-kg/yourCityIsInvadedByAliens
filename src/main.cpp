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

    YKeyboard *pKeyboard = new YKeyboard();
    CoordXY coordCenter = pMainController->getCoordCenter();
    GameAlienShipState *pAlientShipState = pMainController->getGameState()->getAlienShipState();

    // if (!pMainController->showStartDialog()) {
    //     return -1;
    // }

    pMainController->startFpsCounting();
    while (pMainController->getMainState() != MainState::GAME_EXIT) {
        pMainController->getGameState()->updateElapsedTime();
        pMainController->clearWindow();
        pMainController->modifyObjects();
        pMainController->drawObjects();

        SDL_Event event;
        pKeyboard->pollState();
        pMainController->getSoundController()->update();

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                pMainController->setMainState(MainState::GAME_EXIT);
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

                if (pKeyboard->isF12()) {
                    pMainController->toggleFullscreen();
                }

                if (pKeyboard->isF1()) {
                    // pMainController->getWindow()->toggleFullscreen();
                    // TODO show help and pause of the game
                }

                if (pMainController->getMainState() == MainState::WAITING_SPACE) {
                    if (pKeyboard->isSpace()) {
                        pMainController->setMainState(MainState::GAME_ACTION);
                        pMainController->deinitLoaderController();
                        // player
                        pMainController->getWindow()->sortObjectsByPositionZ();
                        pMainController->startAllThreads();
                    }
                } else if (pMainController->getMainState() == MainState::GAME_ACTION) {
                    pAlientShipState->updateStateByKeyboard(pKeyboard);
                }
            }
        }

        if (pMainController->getMainState() == MainState::LOADING) {
            // TODO must be load in other thread
            if (!pMainController->loadGameDataWithProgressBar()) {
                // todo shoe error ?
                pMainController->setMainState(MainState::GAME_EXIT);
            } else {
                pMainController->setMainState(MainState::WAITING_SPACE);
            }
        } else if (pMainController->getMainState() == MainState::GAME_ACTION) {
            // window must move to the player
            /*
                360 - w/2 - 320/2
            */
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

            CoordXY newLeftTop = pAlientShipState->getPosition() - pMainController->getCoordCenter() + CoordXY(320/2, 0);
            pMainController->getGameState()->setCoordLeftTop(newLeftTop);
            pMainController->updatePlayerCoord();
        }
        pMainController->updateFps();
    }

    pMainController->getWindow()->cleanUp();
    // Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}