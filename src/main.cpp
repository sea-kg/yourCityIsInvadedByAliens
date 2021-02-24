#include <iostream>
#include <vector>
#include <fstream>

#include "render_window.h"
#include "wsjcpp_core.h"


int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cerr << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return -1;
    }

    int nWindowWidth = 1280;
    int nWindowHeight = 720;

    GameState stateObjects(nWindowWidth, nWindowHeight);

    RenderWindow window(
        "yourCityIsInvadedByAliens_Tomsk (v0.0.1)",
        stateObjects.windowWidth(),
        stateObjects.windowHeight()
    );

    // load map from json
    std::ifstream ifs("./res/data.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    
    SDL_Texture* buildingTexture = window.loadTexture("res/gfx/building.png");

    nlohmann::json jsonBuildings = jf["buildings"];
    for (auto it = jsonBuildings.begin(); it != jsonBuildings.end(); ++it) {
        // std::cout << it.key() << " | " << it.value() << "\n";
        GameBuilding *pBuilding = new GameBuilding(it.value());
        stateObjects.addBuilding(pBuilding);
        window.addObject(new RenderBuilding2(pBuilding, buildingTexture));
    }

    SDL_Texture* pTextureAlienShip1 = window.loadTexture("res/sprites/alien-ship.png");
    int nCenterX = nWindowWidth/2;
    int nCenterY = nWindowHeight/2;
    CoordXY coordCenter(nCenterX, nCenterY);
    
    // player
    window.addObject(new RenderPlayerAlienShip1(coordCenter, pTextureAlienShip1, 1000));

    // text
    RenderAbsoluteTextBlock *pFpsText = new RenderAbsoluteTextBlock(CoordXY(10, 10), "FPS: ????", 1000);
    window.addObject(pFpsText);

    // coordinates of player
    RenderAbsoluteTextBlock *pCoordText = new RenderAbsoluteTextBlock(CoordXY(10, 40), "x = ? y = ?", 1000);
    window.addObject(pCoordText);

    // object
    // window.addObject(new RenderTriangle(
    //     CoordXY(320, 200),
    //     CoordXY(300, 240),
    //     CoordXY(340, 240)
    // ));
    window.sortObjectsByPositionZ();

    bool gameRunning = true;

    SDL_Event event;

    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    stateObjects.init();

    RenderColor cursorPointer(255,0,0,190);
    RenderMouse *pMouse = new RenderMouse(coordCenter, cursorPointer, 2000);
    pMouse->changeCursorToMoveble();
    window.addObject(pMouse);

    while (gameRunning) {

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  stateObjects.incrementCoordLeftTopX(5); break;
                    case SDLK_RIGHT: stateObjects.incrementCoordLeftTopX(-5); break;
                    case SDLK_UP:    stateObjects.incrementCoordLeftTopY(5); break;
                    case SDLK_DOWN:  stateObjects.incrementCoordLeftTopY(-5); break;
                    case SDLK_w: stateObjects.incrementCoordLeftTopY(5); break;
                    case SDLK_s: stateObjects.incrementCoordLeftTopY(-5); break;
                    case SDLK_a:  stateObjects.incrementCoordLeftTopX(5); break;
                    case SDLK_d: stateObjects.incrementCoordLeftTopX(-5); break;
                    case SDLK_ESCAPE: 
                        if (stateObjects.isMouseCaptured()) {
                            stateObjects.setMouseCaptured(false);
                        }
                        break;
                }
                std::cout << "SDL_KEYDOWN" << std::endl;
            } else if (event.type == SDL_MOUSEMOTION) {
                if (stateObjects.isMouseCaptured()) {
                    CoordXY p0(event.motion.x, event.motion.y);
                    pMouse->updateCoord(p0);
                    pMouse->changeCursorToArrow();
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (!stateObjects.isMouseCaptured()) {
                    stateObjects.setMouseCaptured(true);
                }
            }
        }

        stateObjects.updateElapsedTime();
        window.clear();
        window.modifyObjects(stateObjects);
        window.drawObjects();

        // FPS
        nNumberOfFrames++;
        nElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - nStartTime;
        if (nElapsed > 3000) {
            double nFPS = nNumberOfFrames;
            nFPS /= nElapsed;
            nFPS *= 1000;
            pFpsText->updateText("FPS: " + std::to_string(nFPS));
            std::cout << "FPS: " << nFPS << std::endl;
            nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
            nNumberOfFrames = 0;
            std::string sCoordPlayer = "X=" + std::to_string(stateObjects.getCoordLeftTop().x() + nCenterX)
                + " Y=" + std::to_string(stateObjects.getCoordLeftTop().y() + nCenterY);
            pCoordText->updateText(sCoordPlayer);
        }
    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}