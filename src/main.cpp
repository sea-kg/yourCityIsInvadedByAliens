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
    
    SDL_Texture* pTextureBackground = window.loadTexture("res/gfx/background.png");
    SDL_Texture* pTextureBuildingBlock = window.loadTexture("res/gfx/building-block.png");
    SDL_Texture* pTextureAlienShip1 = window.loadTexture("res/sprites/alien-ship.png");
    SDL_Texture* pTextureCursor = window.loadTexture("res/gfx/mouse-target.png");

    nlohmann::json jsonBuildings = jf["buildings"];
    CoordXY minPointMap(0,0);
    CoordXY maxPointMap(0,0);
    for (auto it = jsonBuildings.begin(); it != jsonBuildings.end(); ++it) {
        // std::cout << it.key() << " | " << it.value() << "\n";
        GameBuilding *pBuilding = new GameBuilding(it.value());
        stateObjects.addBuilding(pBuilding);
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
        window.addObject(pRenderBuilding2);
    }

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
            window.addObject(new RenderBackground(CoordXY(x, y), pTextureBackground, -10));        
        }
    }

    window.sortObjectsByPositionZ();

    bool gameRunning = true;

    SDL_Event event;

    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    stateObjects.init();
    
    RenderMouse *pMouse = new RenderMouse(coordCenter, pTextureCursor, 2000);
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
            pFpsText->updateText("FPS: ~" + std::to_string(int(nFPS)));
            std::cout << "FPS: ~" << int(nFPS) << std::endl;
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