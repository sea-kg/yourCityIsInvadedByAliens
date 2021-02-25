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

    int nWindowWidth = 1280;
    int nWindowHeight = 720;

    GameState gameState(nWindowWidth, nWindowHeight);

    // load map from json
    std::ifstream ifs("./res/data.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    
    SDL_Texture* pTextureBackground = pMainController->loadTexture("res/gfx/background.png");
    SDL_Texture* pTextureBuildingBlock = pMainController->loadTexture("res/gfx/building-block.png");
    SDL_Texture* pTextureAlienShip1 = pMainController->loadTexture("res/sprites/alien-ship.png");
    SDL_Texture* pTextureTank0 = pMainController->loadTexture("res/sprites/tank0.png");
    // SDL_Texture* pTextureRocket = pMainController->loadTexture("res/sprites/rocket.png");
    SDL_Texture* pTextureCursor = pMainController->loadTexture("res/gfx/mouse-target.png");

    nlohmann::json jsonBuildings = jf["buildings"];
    CoordXY minPointMap(0,0);
    CoordXY maxPointMap(0,0);
    for (auto it = jsonBuildings.begin(); it != jsonBuildings.end(); ++it) {
        // std::cout << it.key() << " | " << it.value() << "\n";
        GameBuilding *pBuilding = new GameBuilding(it.value());
        gameState.addBuilding(pBuilding);
        RenderBuilding2 *pRenderBuilding2 = new RenderBuilding2(pBuilding, pTextureBuildingBlock);
        CoordXY min0 = pRenderBuilding2->getMinPoint();
        minPointMap.update(
            std::min(min0.x(), minPointMap.x()),
            std::min(min0.y(), minPointMap.y())
        );
        CoordXY max0 = pRenderBuilding2->getMaxPoint();
        maxPointMap.update(
            std::max(max0.x(), maxPointMap.x()),
            std::max(max0.y(), maxPointMap.y())
        );
        pMainController->getWindow()->addObject(pRenderBuilding2);
    }

    int nCenterX = nWindowWidth/2;
    int nCenterY = nWindowHeight/2;
    CoordXY coordCenter(nCenterX, nCenterY);
    
    // player
    pMainController->getWindow()->addObject(new RenderPlayerAlienShip1(coordCenter, pTextureAlienShip1, 1000));

    // text
    RenderAbsoluteTextBlock *pFpsText = new RenderAbsoluteTextBlock(CoordXY(10, 10), "FPS: ????", 1000);
    pMainController->getWindow()->addObject(pFpsText);

    // coordinates of player
    RenderAbsoluteTextBlock *pCoordText = new RenderAbsoluteTextBlock(CoordXY(10, 40), "x = ? y = ?", 1000);
    pMainController->getWindow()->addObject(pCoordText);

    // background
    // around to 500px
    minPointMap.update(
        minPointMap.x() - ((minPointMap.x() - 500) % 500) - 500,
        minPointMap.y() - ((minPointMap.y() - 500) % 500) - 500
    );
    maxPointMap.update(
        maxPointMap.x() - ((maxPointMap.x() + 500) % 500) + 500,
        maxPointMap.y() - ((maxPointMap.y() + 500) % 500) + 500
    );
    for (int x = minPointMap.x(); x <= maxPointMap.x(); x += 500) {
        for (int y = minPointMap.y(); y <= maxPointMap.y(); y += 500) {
            pMainController->getWindow()->addObject(new RenderBackground(CoordXY(x, y), pTextureBackground, -10));        
        }
    }

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
    gameState.init();
    
    RenderMouse *pMouse = new RenderMouse(coordCenter, pTextureCursor, 2000);
    pMainController->getWindow()->addObject(pMouse);


    pMainController->getWindow()->sortObjectsByPositionZ();
    while (gameRunning) {
        gameState.updateElapsedTime();
        pMainController->getWindow()->clear();
        pMainController->getWindow()->modifyObjects(gameState);
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
                    gameState.incrementCoordLeftTopY(-5);
                } else if (
                    keyboard_state_array[SDL_SCANCODE_UP]
                    && !keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    gameState.incrementCoordLeftTopY(5);
                } else if (
                    !keyboard_state_array[SDL_SCANCODE_UP]
                    && keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    gameState.incrementCoordLeftTopX(5);
                } else if (
                    !keyboard_state_array[SDL_SCANCODE_UP]
                    && !keyboard_state_array[SDL_SCANCODE_LEFT]
                    && keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    gameState.incrementCoordLeftTopX(-5);
                } /*else if (
                    keyboard_state_array[SDL_SCANCODE_UP]
                    && keyboard_state_array[SDL_SCANCODE_LEFT]
                    && !keyboard_state_array[SDL_SCANCODE_RIGHT]
                    && !keyboard_state_array[SDL_SCANCODE_DOWN]
                ) {
                    gameState.incrementCoordLeftTopX(5);
                    gameState.incrementCoordLeftTopY(5);
                }*/

                switch (event.key.keysym.sym) {
                    case SDLK_w: gameState.incrementCoordLeftTopY(5); break;
                    case SDLK_s: gameState.incrementCoordLeftTopY(-5); break;
                    case SDLK_a:  gameState.incrementCoordLeftTopX(5); break;
                    case SDLK_d: gameState.incrementCoordLeftTopX(-5); break;
                    case SDLK_ESCAPE:
                        if (gameState.isMouseCaptured()) {
                            gameState.setMouseCaptured(false);
                        }
                        break;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                if (gameState.isMouseCaptured()) {
                    CoordXY p0(event.motion.x, event.motion.y);
                    pMouse->updateCoord(p0);
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (!gameState.isMouseCaptured()) {
                    gameState.setMouseCaptured(true);
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
            std::string sCoordPlayer = "X=" + std::to_string(gameState.getCoordLeftTop().x() + nCenterX)
                + " Y=" + std::to_string(gameState.getCoordLeftTop().y() + nCenterY);
            pCoordText->updateText(sCoordPlayer);
        }
    }

    pMainController->getWindow()->cleanUp();
    SDL_Quit();
    return 0;
}