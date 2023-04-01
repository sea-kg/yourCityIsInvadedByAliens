
#include "main_controller.h"
#include <iostream>
#include "render.h"
#include "transports/render_tank0.h"
#include "render_ui.h"
#include "ai_tank0.h"
#include "loader_controller.h"
#include "utils_start_dialog.h"
#include <fstream>
#include <algorithm>
#include "render_alienship.h"
#include "roads/render_road0.h"
#include "ycore.h"
#include "ykeyboard.h"
#include "render_background.h"
#include "buildings/render_building_simple.h"
#include "vegetations/render_vegetation_simple.h"
#include "yassets_service.h"
#include "window_yservice.h"
#include <yassets.h>

// MainController

MainController::MainController() {
    TAG = L"MainController";
    m_nProgressBarStatus = 0;
    m_nProgressBarMax = 100;
    m_nMaxClouds = 10000;
    m_pMainAiThread = new MainAiThread();
    m_nCurrentState = MainState::LOADING;
    m_pSettings = findYService<SettingsYService>();
    m_pWindow = findYService<WindowYService>();
}

MainController::~MainController() {
    delete m_pMainAiThread;
}

bool MainController::init() {
    m_pGameState = new GameState(
        m_pWindow->getWidth(),
        m_pWindow->getHeight()
    );
    m_pGameState->setPlayMusic(true);

    if (!this->initSoundController()) {
        return false;
    }
    
    m_pSoundController = new SoundController(
        m_pSettings->getResourceDir(),
        this->getGameState()
    );
    m_pSoundController->init();
    
    m_pLoaderController = new LoaderController(
        m_pWindow->getRenderWindow(),
        m_pGameState
    );

    this->getGameState()->init();

    return true;
}

bool MainController::initSoundController() {
    return true;
}

int MainController::startUI() {
    YKeyboard *pKeyboard = new YKeyboard();
    CoordXY coordCenter = getCoordCenter();
    GameAlienShipState *pAlientShipState = m_pGameState->getAlienShipState();
    startGameLogicThread();

    startFpsCounting();
    while (getMainState() != MainState::GAME_EXIT) {
        long nStartTime = getCurrentTimeInMilliseconds();

        getGameState()->updateElapsedTime();
        clearWindow();
        modifyObjects();
        drawObjects();

        SDL_Event event;
        pKeyboard->pollState();
        getSoundController()->update();

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                setMainState(MainState::GAME_EXIT);
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                YLog::info(TAG, L"SDL_KEYDOWN SDL_KEYUP " + pKeyboard->toString());
                this->handleKeyboardCommand(pKeyboard);
            }
        }

        if (getMainState() == MainState::LOADING) {
            // TODO must be load in other thread
            if (!loadGameDataWithProgressBar()) {
                // todo show error ?
                setMainState(MainState::GAME_EXIT);
            }
            // reset time
            nStartTime = getCurrentTimeInMilliseconds();
            // } else {
            //     std::cout << "MainState::WAITING_SPACE" << std::endl;
            //     setMainState(MainState::WAITING_SPACE);
            // }
        } else if (getMainState() == MainState::GAME_ACTION) {
            // window must move to the player
            /*
                360 - w/2 - 320/2
            */
            int nLeftPad = getCoordCenter().x();
            int nRightPad = getCoordCenter().x() - 320;
            int nTopPad = getCoordCenter().y();
            int nBottomPad = getCoordCenter().y();
            pAlientShipState->move(
                getGameState()->getElapsedTime(),
                getGameState()->getMinPoint(),
                getGameState()->getMaxPoint(),
                nLeftPad,
                nRightPad,
                nTopPad,
                nBottomPad
            );

            CoordXY newLeftTop = pAlientShipState->getPosition() - getCoordCenter() + CoordXY(320/2, 0);
            getGameState()->setCoordLeftTop(newLeftTop);
            updatePlayerCoord();
            
        }
        // normalize framerate to 60 fps
        long nFrameTime = 10 - (nStartTime - getCurrentTimeInMilliseconds());
        if (nFrameTime > 0) {
            // YLog::info(TAG, L"Waiting " + std::to_wstring(nFrameTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(nFrameTime));
        } else {
            YLog::info(TAG, L"Warning " + std::to_wstring(nFrameTime));
        }
        updateFps();
    }

    m_pWindow->getRenderWindow()->cleanUp();
    // Mix_FreeMusic(music);
    SDL_Quit();
    return 0;
}

void* processGameLogicThreadWorker(void *arg) {
    MainController *pThread = (MainController *)arg;
    pThread->runGameLogicThread();
    return 0;
}

void MainController::startGameLogicThread() {
    m_bGameLogicThreadStop = false;
    m_pGameLogicThread = new std::thread(&processGameLogicThreadWorker, (void *)this);
}

void MainController::runGameLogicThread() {
    YLog::info(TAG, L"Starting...");
    YLog::info(TAG, L"TODO Loading assets...");
    auto *pAssets = findYService<YAssetsService>();

    while (!m_bGameLogicThreadStop) {
        std::lock_guard<std::mutex> guard(m_mutexGameLogicThread);
        // YLog::info(TAG, "TODO step...");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    YLog::info(TAG, L"Stopped...");
}

GameState *MainController::getGameState() {
    return m_pGameState;
}

CoordXY MainController::getCoordCenter() {
    return CoordXY(
        m_pWindow->getWidth()/2,
        m_pWindow->getHeight()/2
    );
}

bool MainController::loadGameDataWithProgressBar() {
    m_pLoaderController->init();
    m_pLoaderController->setProgressMax(10);
    m_pLoaderController->setProgressCurrent(0);

    m_pLoaderController->updateText(L"Loading... default map");
    std::cout << L"default/map.json" << std::endl;
    std::wstring sDefaultPath = m_pSettings->getResourceDir() + L"/default";
    YJson jsonDefaultMap(sDefaultPath + L"/map.json");
    if (jsonDefaultMap.isParserFailed()) {
        return false;
    }
    m_minPointMap = CoordXY(0,0);
    m_sMapName = jsonDefaultMap[L"title"].getString();
    m_nMaxClouds = jsonDefaultMap[L"max-clouds"].getNumber();
    m_nMapWidth = jsonDefaultMap[L"width"].getNumber();
    m_nMapHeight = jsonDefaultMap[L"height"].getNumber();
    m_maxPointMap = CoordXY(
        m_nMapWidth,
        m_nMapHeight
    );
    m_pGameState->updatePlayerStartPosition(CoordXY(
        jsonDefaultMap[L"player-start-x"].getNumber(),
        jsonDefaultMap[L"player-start-y"].getNumber()
    ));
    m_pLoaderController->addToProgressCurrent(1);

    auto *pAssets = findYService<YAssetsService>();
    std::wstring sPath = m_pSettings->getResourceDir() + L"/asset-factories/";
    std::vector<std::wstring> vAssets = YCore::getListOfDirs(sPath);
    for (int i = 0; i < vAssets.size(); i++) {
        std::wstring sFactoryPath = sPath + vAssets[i];
        YLog::info(TAG, L"Try loading '" + sFactoryPath + L"'");
        std::wstring sError;
        if (!pAssets->loadAssetFactory(sFactoryPath, sError)) {
            YLog::throw_err(TAG, sError);
        } else {
            YLog::info(TAG, L"Loaded and registered factory '" + vAssets[i] + L"' from " + sFactoryPath);
        }
    }

    m_pLoaderController->updateText(L"Generating background...");
    loadBackgrounds(sDefaultPath, jsonDefaultMap[L"background"]);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Load roads...");
    std::cout << "default/roads.json" << std::endl;
    YJson jsonDefaultRoads(sDefaultPath + L"/roads.json");
    if (jsonDefaultMap.isParserFailed()) {
        return false;
    }
    this->loadRoads(sDefaultPath, jsonDefaultRoads[L"roads"]);
    m_pLoaderController->addToProgressCurrent(1);


    m_pLoaderController->updateText(L"Load buildings...");
    std::cout << L"default/buildings.json" << std::endl;
    YJson jsonDefaultBuildings(sDefaultPath + L"/buildings.json");
    if (jsonDefaultBuildings.isParserFailed()) {
        return false;
    }
    this->loadBuildings(sDefaultPath, jsonDefaultBuildings[L"buildings"]);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Load vegetations...");
    std::cout << "default/vegetations.json" << std::endl;
    YJson jsonDefaultVegetations(sDefaultPath + L"/vegetations.json");
    if (jsonDefaultVegetations.isParserFailed()) {
        return false;
    }
    this->loadVegetations(sDefaultPath, jsonDefaultVegetations[L"vegetations"]);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Load transports...");
    std::cout << "default/transports.json" << std::endl;
    YJson jsonDefaultTransports(sDefaultPath + L"/transports.json");
    if (jsonDefaultTransports.isParserFailed()) {
        return false;
    }
    this->loadTransports(sDefaultPath, jsonDefaultTransports[L"transports"]);
    m_pLoaderController->addToProgressCurrent(1);


    // sDefaultPath
    // default
    m_pLoaderController->updateText(L"Loading... textures");

    m_pWindow->getRenderWindow()->loadTextureBioplast(m_pSettings->getResourceDir() + L"/asset-factories/bioplasm1/bioplasm.png");

    // app
    m_pTextureLeftPanel = m_pWindow->getRenderWindow()->loadTexture(m_pSettings->getResourceDir() + L"/app/textures/left-panel-info.png");
    m_pTexturePlayerPower0 = m_pWindow->getRenderWindow()->loadTexture(m_pSettings->getResourceDir() + L"/app/textures/player-power.png");

    this->loadAlienShip(sDefaultPath);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Loading... buildings textures");
    // TODO remove
    std::vector<std::wstring> vBuildings = YCore::getListOfDirs(m_pSettings->getResourceDir() + L"/buildings");
    for (int i = 0; i < vBuildings.size(); i++) {
        std::wstring sName = vBuildings[i];
        std::wstring sPathTexture = m_pSettings->getResourceDir() + L"/buildings/" + sName + L"/texture.png";
        if (!YCore::fileExists(sPathTexture)) {
            YLog::err(TAG, L"Not found " + sPathTexture);
            m_pLoaderController->updateText(L"Not found " + sPathTexture);
            return false;
        }
        m_mapBuildingsTextures[sName] = m_pWindow->getRenderWindow()->loadTexture(sPathTexture.c_str());
    }
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Loading... buildings");

    m_pGameState->setMinPoint(m_minPointMap);
    m_pGameState->setMaxPoint(m_maxPointMap);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Generating clouds...");
    this->generateClouds();
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Prepare panels...");
    m_pWindow->getRenderWindow()->addPanelsObject(
        new RenderLeftPanelInfo(
            m_pTextureLeftPanel,
            new RenderPlayerPower(m_pTexturePlayerPower0, m_pGameState->getAlienShipState()),
            5000
        )
    );

    

    // text
    m_pFpsText = pAssets->createAsset<YAssetText>(L"text1");
    m_pFpsText->setOrderZ(5001);
    m_pFpsText->setAbsolutePosition(true);
    m_pFpsText->setPosition(m_pWindow->getWidth() - 270, 20);
    m_pFpsText->setText(L"FPS: ...");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pFpsText);

    // coordinates of player
    m_pCoordText = pAssets->createAsset<YAssetText>(L"text1");
    m_pCoordText->setOrderZ(5001);
    m_pCoordText->setAbsolutePosition(true);
    m_pCoordText->setPosition(m_pWindow->getWidth() - 270, 50);
    m_pCoordText->setText(L"x = ? y = ?");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pCoordText);

    m_pLoaderController->addToProgressCurrent(1);
    m_pLoaderController->updateText(L"Press 'space' for continue...");
    YLog::info(TAG, L"MainState::WAITING_SPACE");
    this->setMainState(MainState::WAITING_SPACE);
    return true;
}

void MainController::deinitLoaderController() {
    m_pLoaderController->deinit();
}

bool MainController::showStartDialog() {
     UtilsStartDialog dialog(
        m_pSettings->getResourceDir(),
        m_pWindow->getRenderWindow(),
        m_pGameState
    );
    dialog.init();
    return dialog.start();
}

void MainController::startAllThreads() {
    m_pMainAiThread->start();
}

void MainController::handleKeyboardCommand(YKeyboard *pKeyboard) {
    if (pKeyboard->isEscape()) {
        if (isFullscreen()) {
            toggleFullscreen();
        } else {
            // TODO pause
        }
    }

    if (pKeyboard->isF12()) {
        toggleFullscreen();
    }

    if (pKeyboard->isF1()) {
        // pMainController->getWindow()->toggleFullscreen();
        // TODO show help and pause of the game
        // if (!this->showStartDialog()) {
        //     return;
        // }
    }

    if (getMainState() == MainState::WAITING_SPACE) {
        if (pKeyboard->isSpace()) {
            YLog::info(TAG, L"pKeyboard->isSpace()");
            setMainState(MainState::GAME_ACTION);
            deinitLoaderController();
            // player
            m_pWindow->getRenderWindow()->sortObjectsByPositionZ();
            startAllThreads();
        }
    } else if (getMainState() == MainState::GAME_ACTION) {
        m_pGameState->getAlienShipState()->updateStateByKeyboard(pKeyboard);
    }
}

bool MainController::isFullscreen() {
    return m_pWindow->getRenderWindow()->isFullscreen();
}

void MainController::toggleFullscreen() {
    m_pWindow->toggleFullscreen();
    m_pGameState->updateWindowSize(m_pWindow->getWidth(),m_pWindow->getHeight());
}

void MainController::clearWindow() {
    m_pWindow->getRenderWindow()->clear();
}

void MainController::modifyObjects() {
    m_pWindow->getRenderWindow()->modifyObjects(*m_pGameState);

    CoordXY p0 = m_pGameState->getAlienShipState()->getPosition();
    // calculate intersection rockets and player
    for (int i = 0; i < m_pWindow->getRenderWindow()->m_vRockets.size(); i++) {
        GameRocketState *pRocket = m_pWindow->getRenderWindow()->m_vRockets[i];
        if (pRocket->isExploded() || pRocket->hasDestroyed()) {
            continue;
        }
        YPos p1 = pRocket->getPosition();
        // distance
        double nDistance = p1.getDistance(YPos(p0.x(), p0.y()));
        if (nDistance < 30.0) {
            pRocket->explode();
            m_pGameState->getAlienShipState()->rocketAttack(pRocket);
        }
    }

    for (int i = 0; i < m_pWindow->getRenderWindow()->m_vRockets.size(); i++) {
        GameRocketState *pRocket = m_pWindow->getRenderWindow()->m_vRockets[i];
        if (pRocket->isExploded() || pRocket->hasDestroyed()) {
            continue;
        }
        YPos p1 = pRocket->getPosition();

        for (int b = 0; b < m_pWindow->getRenderWindow()->m_vBioplasts.size(); b++) {
            GameBioplastState *pBioplast = m_pWindow->getRenderWindow()->m_vBioplasts[b];
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
    m_pWindow->getRenderWindow()->drawObjects(*m_pGameState);
}

void MainController::updatePlayerCoord() {
    const CoordXY &playerCoord = m_pGameState->getAlienShipState()->getPosition();
    std::wstring sCoordPlayer = L"X=" + std::to_wstring(playerCoord.x())
            + L"\nY=" + std::to_wstring(playerCoord.y());
    m_pCoordText->setText(sCoordPlayer);
}

void MainController::startFpsCounting() {
    m_nFpsNumberOfFrames = 0;
    m_nFpsStartTime = getCurrentTimeInMilliseconds();
    m_nFpsElapsed = 0;
}

void MainController::updateFps() {
    m_nFpsNumberOfFrames++;
    m_nFpsElapsed = getCurrentTimeInMilliseconds() - m_nFpsStartTime;
    if (m_nFpsElapsed > 1000) {
        double nFPS = m_nFpsNumberOfFrames;
        nFPS /= m_nFpsElapsed;
        nFPS *= 1000;
        this->updateFpsValue(nFPS);
        m_nFpsStartTime = getCurrentTimeInMilliseconds();
        m_nFpsNumberOfFrames = 0;
    }
}

void MainController::updateFpsValue(int nFps) {
    m_pFpsText->setText(L"FPS: ~" + std::to_wstring(nFps));
    std::cout << "FPS: ~" << nFps << std::endl;
}

SoundController *MainController::getSoundController() {
    return m_pSoundController;
}

MainState MainController::getMainState() {
    return m_nCurrentState;
}

void MainController::setMainState(const MainState &newMainState) {
    m_nCurrentState = newMainState;

    if (m_nCurrentState == MainState::LOADING) {
        YLog::info(TAG, L"setMainState MainState::LOADING");
    } else if (m_nCurrentState == MainState::WAITING_SPACE) {
        YLog::info(TAG, L"setMainState MainState::WAITING_SPACE");
    } else if (m_nCurrentState == MainState::GAME_ACTION) {
        YLog::info(TAG, L"setMainState MainState::WAITING_SPACE");
    } else if (m_nCurrentState == MainState::GAME_EXIT) {
        YLog::info(TAG, L"setMainState MainState::GAME_EXIT");
    } else {
        YLog::info(TAG, L"setMainState ???");
    }
}

void MainController::loadBackgrounds(
    const std::wstring &sDefaultPath,
    const YJsonObject &jsonBackground
) {
    for (int i = 0; i < jsonBackground.length(); i++) {
        const YJsonObject &item = jsonBackground[i];
        std::wstring sTexturePath = sDefaultPath + L"/" + item[L"texture"].getString();
        if (!YCore::fileExists(sTexturePath)) {
            YLog::throw_err(TAG, L"File '" + sTexturePath + L"' not found");
        }
        SDL_Texture* pTextureBackground = m_pWindow->getRenderWindow()->loadTexture(sTexturePath);
        int nTextureWidth = item[L"width"].getNumber();
        int nTextureHeight = item[L"height"].getNumber();
        const YJsonObject &fillRegion = item[L"fill-region"];
        CoordXY startXY(
            fillRegion[L"start-x"].getNumber(),
            fillRegion[L"start-y"].getNumber()
        );
        CoordXY endXY(
            fillRegion[L"end-x"].getNumber(),
            fillRegion[L"end-y"].getNumber()
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
            m_pWindow->getRenderWindow()->addGroundObject(new RenderBackground(CoordXY(x, y), pTextureBackground));        
        }
    }
}

void MainController::generateClouds() {
    auto *pAssets = findYService<YAssetsService>();

    for (int i = 0; i < m_nMaxClouds; i++) {
        int nX = std::rand() % m_nMapWidth;
        nX += m_minPointMap.x();
        int nY = std::rand() % m_nMapHeight;
        nY += m_minPointMap.y();
        
        auto *pClouds = pAssets->createAsset<YAssetClouds>(L"clouds1");
        pClouds->setPosition(nX, nY);
        m_pWindow->getRenderWindow()->addCloudsObject(pClouds);

        // int nCloudType = std::rand() % m_vTexturesClouds.size();
        // m_pWindow->getRenderWindow()->addCloudsObject(new RenderCloud0(
        //     pCloud0State,
        //     m_vTexturesClouds[nCloudType],
        //     1000
        // ));
    }
}

void MainController::loadRoads(
    const std::wstring &sDefaultPath,
    const YJsonObject &jsonRoads
) {
    for (int i = 0; i < jsonRoads.length(); i++) {
        const YJsonObject &item = jsonRoads[i];
        std::wstring sTexturePath = sDefaultPath + L"/" + item[L"texture"].getString();
        if (!YCore::fileExists(sTexturePath)) {
            YLog::throw_err(TAG, L"File '" + sTexturePath + L"' not found");
        }
        SDL_Texture* pTextureRoads = m_pWindow->getRenderWindow()->loadTexture(sTexturePath);
        int nTextureWidth = item[L"width"].getNumber();
        int nTextureHeight = item[L"height"].getNumber();
        const YJsonObject &fillList = item[L"fill"];

        for (int n = 0; n < fillList.length(); n++) {
            const YJsonObject &roadItem = fillList[n];
            int nX = roadItem[L"x"].getNumber();
            int nY = roadItem[L"y"].getNumber();
            std::wstring sRoadPart = roadItem[L"road-part"].getString();
            m_pWindow->getRenderWindow()->addRoadsObject(new RenderRoad0(
                CoordXY(nX, nY),
                pTextureRoads,
                convertStringToRoadPart(sRoadPart)
            ));
        }
    }
}

void MainController::loadAlienShip(
    const std::wstring &sDefaultPath
) {
    
    std::wstring sFilenamePng = sDefaultPath + L"/sprites/alien-ship0.png";
    if (!YCore::fileExists(sFilenamePng)) {
        YLog::throw_err(TAG, L"File not exists " + sFilenamePng);
    }

    SDL_Texture* pTextureAlienShip1 = m_pWindow->getRenderWindow()->loadTexture(sFilenamePng);

    std::wstring sFilenameJson = sDefaultPath + L"/sprites/alien-ship0.json";
    if (!YCore::fileExists(sFilenameJson)) {
        YLog::throw_err(TAG, L"File not exists " + sFilenameJson);
    }
    
    YJson jsonAlienShip(sFilenameJson);
    if (jsonAlienShip.isParserFailed()) {
        YLog::throw_err(TAG, L"Could not parse file " + sFilenameJson);
    }

    // shadow
    if (jsonAlienShip[L"shadow"].getString() == L"yes") {
        m_pWindow->getRenderWindow()->addFlyingShadowObject(
        new RenderAlienShip0(
                m_pGameState->getAlienShipState(),
                jsonAlienShip,
                true,
                pTextureAlienShip1,
                1000
            )
        );
    }

    // ship
    m_pWindow->getRenderWindow()->addFlyingObject(
        new RenderAlienShip0(
            m_pGameState->getAlienShipState(),
            jsonAlienShip,
            false,
            pTextureAlienShip1,
            1000
        )
    );
}


void MainController::loadBuildings(
    const std::wstring &sDefaultPath,
    const YJsonObject &jsonRoads
) {
    for (int i = 0; i < jsonRoads.length(); i++) {
        const YJsonObject &item = jsonRoads[i];
        std::wstring sTexturePath = sDefaultPath + L"/" + item[L"texture"].getString();
        if (!YCore::fileExists(sTexturePath)) {
            YLog::throw_err(TAG, L"File '" + sTexturePath + L"' not found");
        }
        SDL_Texture* pTextureBuilding = m_pWindow->getRenderWindow()->loadTexture(sTexturePath);
        int nTextureWidth = item[L"width"].getNumber();
        int nTextureHeight = item[L"height"].getNumber();
        const YJsonObject &fillList = item[L"fill"];

        for (int n = 0; n < fillList.length(); n++) {
            const YJsonObject &roadItem = fillList[n];
            int nX = roadItem[L"x"].getNumber();
            int nY = roadItem[L"y"].getNumber();
            m_pWindow->getRenderWindow()->addBuildingsObject(new RenderBuildingSimple(
                YPos(nX, nY),
                nTextureWidth,
                nTextureHeight,
                pTextureBuilding
            ));
        }
    }
}

void MainController::loadVegetations(
    const std::wstring &sDefaultPath,
    const YJsonObject &jsonVegetations
) {
    for (int i = 0; i < jsonVegetations.length(); i++) {
        const YJsonObject &item = jsonVegetations[i];
        std::wstring sTexturePath = sDefaultPath + L"/" + item[L"texture"].getString();
        if (!YCore::fileExists(sTexturePath)) {
            YLog::throw_err(TAG, L"File '" + sTexturePath + L"' not found");
        }
        SDL_Texture* pTextureBuilding = m_pWindow->getRenderWindow()->loadTexture(sTexturePath);
        int nTextureWidth = item[L"width"].getNumber();
        int nTextureHeight = item[L"height"].getNumber();
        const YJsonObject &fillList = item[L"fill"];

        for (int n = 0; n < fillList.length(); n++) {
            const YJsonObject &roadItem = fillList[n];
            int nX = roadItem[L"x"].getNumber();
            int nY = roadItem[L"y"].getNumber();
            m_pWindow->getRenderWindow()->addVegetationObject(new RenderVegetationSimple(
                YPos(nX, nY),
                nTextureWidth,
                nTextureHeight,
                pTextureBuilding
            ));
        }
    }
}

void MainController::loadTransports(
    const std::wstring &sDefaultPath,
    const YJsonObject &jsonTransports
) {
    for (int i = 0; i < jsonTransports.length(); i++) {
        const YJsonObject &item = jsonTransports[i];
        std::wstring sSpritePath = sDefaultPath + L"/" + item[L"sprite"].getString();
        std::wstring sSpriteRocketPath = sDefaultPath + L"/" + item[L"sprite-rocket"].getString();
        if (!YCore::fileExists(sSpritePath)) {
            YLog::throw_err(TAG, L"File '" + sSpritePath + L"' not found");
        }
        SDL_Texture* pSprite = m_pWindow->getRenderWindow()->loadTexture(sSpritePath);

        if (!YCore::fileExists(sSpriteRocketPath)) {
            YLog::throw_err(TAG, L"File '" + sSpriteRocketPath + L"' not found");
        }
        SDL_Texture* pSpriteRocket = m_pWindow->getRenderWindow()->loadTexture(sSpriteRocketPath);

        int nSpriteWidth = item[L"sprite-width"].getNumber();
        int nSpriteHeight = item[L"sprite-height"].getNumber();
        const YJsonObject &fillList = item[L"fill"];

        for (int n = 0; n < fillList.length(); n++) {
            const YJsonObject &roadItem = fillList[n];
            int nX = roadItem[L"x"].getNumber();
            int nY = roadItem[L"y"].getNumber();

            GameTank0State *pTankState = new GameTank0State(CoordXY(nX,nY));
            AiTank0 *pAiTank0 = new AiTank0(pTankState);

            m_pWindow->getRenderWindow()->addTransportsObject(new RenderTank0(
                pTankState,
                pSprite,
                pSpriteRocket,
                nSpriteWidth,
                nSpriteHeight
            ));
            m_pMainAiThread->addAiObject(pAiTank0);
        }
    }
}
