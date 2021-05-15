
#include "main_controller.h"
#include <iostream>
#include "render.h"
#include "render_tank0.h"
#include "render_ui.h"
#include "ai_tank0.h"
#include "utils_loader_screen.h"
#include "utils_start_dialog.h"
#include <fstream>
#include <algorithm>
#include "render_player_alient_ship.h"
#include "render_cloud0.h"
#include "render_road0.h"
#include "wsjcpp_core.h"
#include "ycore.h"
#include "ykeyboard.h"
#include "game_cloud0_state.h"
#include "render_background.h"

// MainController

MainController::MainController(const std::string &sWindowName) {
    m_sWindowName = sWindowName;
    m_nWindowWidth = 1280;
    m_nWindowHeight = 720;
    m_pRenderWindow = nullptr;
    m_nProgressBarStatus = 0;
    m_nProgressBarMax = 100;
    m_nMaxClouds = 10000;
    m_sResourceDir = "./res";
    m_pAlientShipState = nullptr;
    TAG = "MainController";
    m_pMainAiThread = new MainAiThread();
}

bool MainController::findResourceDir() {
    std::vector<std::string> vPaths;
    vPaths.push_back("/usr/share/yourCityIsInvadedByAliens_Tomsk");
    vPaths.push_back("./res");
    m_sResourceDir = "";
    for (int i = 0; i < vPaths.size(); i++) {
        if (YCore::dirExists(vPaths[i])) {
            m_sResourceDir = vPaths[i];
            break;
        }
    }
    if (m_sResourceDir == "") {
        std::cerr << "Not found resource dir (./res by default)" << std::endl;
        return false;
    }
    std::cout << "Resource dir: " << m_sResourceDir << std::endl;
    return true;
}

std::string MainController::getResourceDir() {
    return m_sResourceDir;
}

bool MainController::initSDL2() {
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cerr << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return false;
    }
    
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }
    int result = 0;
    int flags = MIX_INIT_OGG;
    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }
    return true;
}

bool MainController::initRenderWindow() {
    
    if (!this->initSDL2()) {
        return false;
    }

    m_pRenderWindow = new RenderWindow(
        m_sWindowName.c_str(),
        m_nWindowWidth,
        m_nWindowHeight
    );
    m_pGameState = new GameState(
        m_nWindowWidth,
        m_nWindowHeight
    );
    return true;
}

RenderWindow *MainController::getWindow() {
    return m_pRenderWindow;
}

GameState *MainController::getGameState() {
    return m_pGameState;
}

CoordXY MainController::getCoordCenter() {
    return CoordXY(
        m_nWindowWidth/2,
        m_nWindowHeight/2
    );
}

bool MainController::loadGameDataWithProgressBar() {
    UtilsLoaderScreen loader(
        m_sResourceDir,
        m_pRenderWindow,
        m_pGameState
    );
    loader.init();
    loader.setProgressMax(10);
    loader.setProgressCurrent(0);

    loader.updateText("Loading... default map");
    std::cout << "default/map.json" << std::endl;
    std::string sDefaultPath = m_sResourceDir + "/default";
    YJson jsonDefaultMap(sDefaultPath + "/map.json");
    if (jsonDefaultMap.isParserFailed()) {
        return false;
    }
    m_minPointMap = CoordXY(0,0);
    m_sMapName = jsonDefaultMap["title"].getString();
    m_nMaxClouds = jsonDefaultMap["max-clouds"].getNumber();
    m_nMapWidth = jsonDefaultMap["width"].getNumber();
    m_nMapHeight = jsonDefaultMap["height"].getNumber();
    m_maxPointMap = CoordXY(
        m_nMapWidth,
        m_nMapHeight
    );
    m_playerStartPosition = CoordXY(
        jsonDefaultMap["player-start-x"].getNumber(),
        jsonDefaultMap["player-start-y"].getNumber()
    );
    loader.addToProgressCurrent(1);

    loader.updateText("Generating background...");
    loadBackgrounds(sDefaultPath, jsonDefaultMap["background"]);
    loader.addToProgressCurrent(1);

    loader.updateText("Load roads...");
    this->loadRoads(sDefaultPath, jsonDefaultMap["roads"]);
    loader.addToProgressCurrent(1);

    // sDefaultPath
    // default
    loader.updateText("Loading... textures");
    m_pTextureAlienShip1 = m_pRenderWindow->loadTexture(m_sResourceDir + "/default/sprites/alien-ship.png");
    m_pTextureTank0 = m_pRenderWindow->loadTexture(m_sResourceDir + "/default/sprites/tank0.png");
    m_pRenderWindow->loadTextureRocket(m_sResourceDir + "/default/sprites/tank0-rocket.png");
    m_pRenderWindow->loadTextureBioplast(m_sResourceDir + "/default/sprites/alien-bioplast.png");
    m_pTextureCloud0 = m_pRenderWindow->loadTexture(m_sResourceDir + "/default/textures/cloud0.png");
    m_pTextureRoad0 = m_pRenderWindow->loadTexture(m_sResourceDir + "/default/textures/road0.png");

    // app
    m_pTextureLeftPanel = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/left-panel-info.png");
    m_pTexturePlayerPower0 = m_pRenderWindow->loadTexture(m_sResourceDir + "/app/textures/player-power.png");

    m_pAlientShipState = new GameAlienShipState(m_playerStartPosition);
    loader.addToProgressCurrent(1);

    loader.updateText("Loading... buildings textures");

    std::vector<std::string> vBuildings = YCore::getListOfDirs(m_sResourceDir + "/buildings");
    for (int i = 0; i < vBuildings.size(); i++) {
        std::string sName = vBuildings[i];
        std::string sPathTexture = m_sResourceDir + "/buildings/" + sName + "/texture.png";
        if (!YCore::fileExists(sPathTexture)) {
            YLog::err(TAG, "Not found " + sPathTexture);
            loader.updateText("Not found " + sPathTexture);
            return false;
        }
        m_mapBuildingsTextures[sName] = m_pRenderWindow->loadTexture(sPathTexture.c_str());
    }
    loader.addToProgressCurrent(1);

    loader.updateText("Loading... buildings");
    
    SDL_Texture* pTextureTower0 = m_mapBuildingsTextures["tower0"];

    // load map from json
    std::cout << "Data" << std::endl;
    YJson jsonData(m_sResourceDir + "/data.json");
    
    YJsonObject jsonBuildings = jsonData["buildings"];
    std::vector<std::string> vKeys = jsonBuildings.getKeys();
    loader.addToProgressMax(vKeys.size());
    for (int i = 0; i < vKeys.size(); i++) {
        std::string sKey = vKeys[i];
        // std::cout << sKey << std::endl;
        GameBuilding *pBuilding = new GameBuilding(jsonBuildings[sKey]);
        m_pGameState->addBuilding(pBuilding);
        RenderBuilding2 *pRenderBuilding2 = new RenderBuilding2(pBuilding, pTextureTower0);
        CoordXY min0 = pRenderBuilding2->getMinPoint();
        CoordXY max0 = pRenderBuilding2->getMaxPoint();
        m_pRenderWindow->addBuildingsObject(pRenderBuilding2);
        loader.addToProgressCurrent(1);
    }

    m_pGameState->setMinPoint(m_minPointMap);
    m_pGameState->setMaxPoint(m_maxPointMap);
    loader.addToProgressCurrent(1);

    loader.updateText("Generating enimies...");
    this->generateTanks();
    loader.addToProgressCurrent(1);

    loader.updateText("Generating clouds...");
    this->generateClouds();
    loader.addToProgressCurrent(1);

    loader.updateText("Prepare panels...");
    m_pRenderWindow->addPanelsObject(
        new RenderLeftPanelInfo(
            m_pTextureLeftPanel,
            new RenderPlayerPower(m_pTexturePlayerPower0, m_pAlientShipState),
            5000
        )
    );

    // text
    m_pFpsText = new RenderAbsoluteTextBlock(CoordXY(m_nWindowWidth - 270, 20), "FPS: ...", 5001);
    m_pRenderWindow->addPanelsObject(m_pFpsText);

    // coordinates of player
    m_pCoordText = new RenderAbsoluteTextBlock(CoordXY(m_nWindowWidth - 270, 40), "x = ? y = ?", 5001);
    m_pRenderWindow->addPanelsObject(m_pCoordText);

    
    m_pRenderWindow->addFlyingObject(
        new RenderPlayerAlienShip(
            m_pAlientShipState,
            m_pTextureAlienShip1,
            1000
        )
    );
    loader.addToProgressCurrent(1);

    return true;
}

bool MainController::showStartDialog() {
     UtilsStartDialog dialog(
        m_sResourceDir,
        m_pRenderWindow,
        m_pGameState
    );
    dialog.init();
    return dialog.start();
}

void MainController::startAllThreads() {
    m_pMainAiThread->start();
}

bool MainController::isFullscreen() {
    return m_pRenderWindow->isFullscreen();
}

void MainController::toggleFullscreen() {
    m_pRenderWindow->toggleFullscreen();
    int w,h;
    m_pRenderWindow->getWindowSize(&w,&h);
    std::cout << "w,h = (" << w << "," << h << ")";
    m_pGameState->updateWindowSize(w,h);
    m_nWindowWidth = w;
    m_nWindowHeight = h;
}

void MainController::clearWindow() {
    m_pRenderWindow->clear();
}

void MainController::modifyObjects() {
    m_pRenderWindow->modifyObjects(*m_pGameState);

    CoordXY p0 = m_pAlientShipState->getPosition();
    // calculate intersection rockets and player
    for (int i = 0; i < m_pRenderWindow->m_vRockets.size(); i++) {
        GameRocketState *pRocket = m_pRenderWindow->m_vRockets[i];
        if (pRocket->isExploded() || pRocket->hasDestroyed()) {
            continue;
        }
        YPos p1 = pRocket->getPosition();
        // distance
        double nDistance = p1.getDistance(YPos(p0.x(), p0.y()));
        if (nDistance < 30.0) {
            pRocket->explode();
            m_pAlientShipState->rocketAttack(pRocket);
        }
    }

    for (int i = 0; i < m_pRenderWindow->m_vRockets.size(); i++) {
        GameRocketState *pRocket = m_pRenderWindow->m_vRockets[i];
        if (pRocket->isExploded() || pRocket->hasDestroyed()) {
            continue;
        }
        YPos p1 = pRocket->getPosition();

        for (int b = 0; b < m_pRenderWindow->m_vBioplasts.size(); b++) {
            GameBioplastState *pBioplast = m_pRenderWindow->m_vBioplasts[b];
            CoordXY p2 = pBioplast->getPosition();
            // distance
            float nDistance = p1.getDistance(YPos(p2.x(), p2.y()));
            if (nDistance < 15.0) {
                pRocket->explode();
                // m_pAlientShipState->rocketAttack();
            }
        }
    }
}

void MainController::drawObjects() {
    m_pRenderWindow->drawObjects(*m_pGameState);
}

void MainController::updatePlayerCoord() {
    const CoordXY &playerCoord = m_pAlientShipState->getPosition();
    std::string sCoordPlayer = "X=" + std::to_string(playerCoord.x())
            + " Y=" + std::to_string(playerCoord.y());
    m_pCoordText->updateText(sCoordPlayer);
}

void MainController::updateFpsValue(int nFps) {
    m_pFpsText->updateText("FPS: ~" + std::to_string(nFps));
    std::cout << "FPS: ~" << nFps << std::endl;
}

GameAlienShipState *MainController::getGameAlienShipState() {
    return m_pAlientShipState;
}

void MainController::loadBackgrounds(
    const std::string &sDefaultPath,
    const YJsonObject &jsonBackground
) {
    for (int i = 0; i < jsonBackground.length(); i++) {
        const YJsonObject &item = jsonBackground[i];
        std::string sTexturePath = sDefaultPath + "/" + item["texture"].getString();
        if (!YCore::fileExists(sTexturePath)) {
            YLog::throw_err(TAG, "File '" + sTexturePath + "' not found");
        }
        SDL_Texture* pTextureBackground = m_pRenderWindow->loadTexture(sTexturePath);
        int nTextureWidth = item["width"].getNumber();
        int nTextureHeight = item["height"].getNumber();
        const YJsonObject &fillRegion = item["fill-region"];
        CoordXY startXY(
            fillRegion["start-x"].getNumber(),
            fillRegion["start-y"].getNumber()
        );
        CoordXY endXY(
            fillRegion["end-x"].getNumber(),
            fillRegion["end-y"].getNumber()
        );
        generateBackground(
            pTextureBackground,
            nTextureWidth,
            nTextureHeight,
            startXY,
            endXY
        );
    }
}

void MainController::generateBackground(
    SDL_Texture* pTextureBackground,
    int nTextureWidth,
    int nTextureHeight,
    const CoordXY &startXY,
    const CoordXY &endXY
) {
    for (int x = startXY.x(); x <= endXY.x(); x += nTextureWidth) {
        for (int y = startXY.y(); y <= endXY.y(); y += nTextureHeight) {
            m_pRenderWindow->addGroundObject(new RenderBackground(CoordXY(x, y), pTextureBackground));        
        }
    }
}

void MainController::generateTanks() {

    for (int i = 0; i < 25; i++) {
        int nXpos = std::rand() % m_nWindowWidth;
        int nYpos = std::rand() % m_nWindowHeight;
        
        GameTank0State *pTankState = new GameTank0State(CoordXY(nXpos,nYpos));
        AiTank0 *pAiTank0 = new AiTank0(pTankState);

        m_pRenderWindow->addTransportsObject(new RenderTank0(
            pTankState,
            m_pTextureTank0,
            1000
        ));

        m_pMainAiThread->addAiObject(pAiTank0);
    }
}


void MainController::generateClouds() {
    for (int i = 0; i < m_nMaxClouds; i++) {
        int nXpos = std::rand() % m_nMapWidth;
        nXpos += m_minPointMap.x();
        int nYpos = std::rand() % m_nMapHeight;
        nYpos += m_minPointMap.y();
        
        auto *pCloud0State = new GameCloud0State(CoordXY(nXpos,nYpos));

        m_pRenderWindow->addCloudsObject(new RenderCloud0(
            pCloud0State,
            m_pTextureCloud0,
            1000
        ));
    }
}

void MainController::loadRoads(
    const std::string &sDefaultPath,
    const YJsonObject &jsonRoads
) {
    for (int i = 0; i < jsonRoads.length(); i++) {
        const YJsonObject &item = jsonRoads[i];
        std::string sTexturePath = sDefaultPath + "/" + item["texture"].getString();
        if (!YCore::fileExists(sTexturePath)) {
            YLog::throw_err(TAG, "File '" + sTexturePath + "' not found");
        }
        SDL_Texture* pTextureRoads = m_pRenderWindow->loadTexture(sTexturePath);
        int nTextureWidth = item["width"].getNumber();
        int nTextureHeight = item["height"].getNumber();
        const YJsonObject &fillList = item["fill"];

        for (int n = 0; n < fillList.length(); n++) {
            const YJsonObject &roadItem = fillList[n];
            int nX = roadItem["x"].getNumber();
            int nY = roadItem["y"].getNumber();
            std::string sRoadPart = roadItem["road-part"].getString();
            m_pRenderWindow->addRoadsObject(new RenderRoad0(
                CoordXY(nX, nY),
                pTextureRoads,
                convertStringToRoadPart(sRoadPart)
            ));
        }
    }
}


