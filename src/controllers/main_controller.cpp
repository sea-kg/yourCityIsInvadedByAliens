#include <fstream>
#include <algorithm>
#include <iostream>
#include "main_controller.h"
#include "render.h"
#include "render_ui.h"
#include "ai_tank0.h"
#include "loader_controller.h"
#include "utils_start_dialog.h"
#include "render_alienship.h"
#include "ycore.h"
#include "ykeyboard.h"
#include "buildings/render_building_simple.h"
#include "vegetations/render_vegetation_simple.h"
#include "yassets_service.h"
#include "window_yservice.h"
#include "map_yservice.h"
#include "Roads2DGenerator.h"
#include "yassets.h"

// MainController

MainController::MainController() {
    TAG = L"MainController";
    m_nProgressBarStatus = 0;
    m_nProgressBarMax = 100;
    m_pMainAiThread = new MainAiThread();
    m_nCurrentState = MainState::LOADING;
    m_pSettings = findYService<SettingsYService>();
    m_pWindow = findYService<WindowYService>();
    m_pMap = findYService<MapYService>();
    m_nCurrentTakeAlienBerry = -1;
    m_nTakedPlayerBerries = 0;
    m_nScoreforRandomShooting;
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
    YPos coordCenter = getCoordCenter();
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
                // YLog::info(TAG, L"SDL_KEYDOWN SDL_KEYUP " + pKeyboard->toString());
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
        } else if (getMainState() == MainState::GAME_HELP) {
            // TODO render dialog
        } else if (getMainState() == MainState::GAME_ACTION) {
            // window must move to the player
            /*
                360 - w/2 - 320/2
            */
            int nLeftPad = getCoordCenter().getX();
            int nRightPad = getCoordCenter().getX() - 320;
            int nTopPad = getCoordCenter().getY();
            int nBottomPad = getCoordCenter().getY();
            pAlientShipState->move(
                getGameState()->getElapsedTime(),
                getGameState()->getMinPoint(),
                getGameState()->getMaxPoint(),
                0, // nLeftPad,
                0, // nRightPad,
                0, // nTopPad,
                0 // nBottomPad
            );
            YPos newLeftTop = pAlientShipState->getPosition() - getCoordCenter() + YPos(320/2, 0);
            getGameState()->setCoordLeftTop(newLeftTop);
            updatePlayerCoord();
            updateScore();

            if (pAlientShipState->getHelthPoints() <= 0) {
                setMainState(MainState::GAME_OVER);
            }
        } else if (getMainState() == MainState::GAME_OVER) {
            m_pGameState->getAlienShipState()->setShooting(false);
            m_pSoundController->stopTakeBerry();
            m_nCurrentTakeAlienBerry = -1;
            m_pDialogGameOver->setShow(true);
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

YPos MainController::getCoordCenter() {
    return YPos(
        m_pWindow->getWidth()/2,
        m_pWindow->getHeight()/2
    );
}

bool MainController::loadGameDataWithProgressBar() {
    m_pLoaderController->init();
    m_pLoaderController->setProgressMax(12);
    m_pLoaderController->setProgressCurrent(0);

    m_pLoaderController->updateText(L"Loading... default map");
    m_cfgDefaultMap = ConfigDefaultMap::loadConfig(m_pSettings->getResourceDir() + L"/default");

    std::wstring sDefaultPath = m_pSettings->getResourceDir() + L"/default";

    m_pMap->setMapSize(m_cfgDefaultMap->getWidth(), m_cfgDefaultMap->getHeight());

    m_pGameState->updatePlayerStartPosition(m_cfgDefaultMap->getPlayerStartPos());
    m_pLoaderController->addToProgressCurrent(1);

    auto *pAssets = findYService<YAssetsService>();
    std::wstring sPath = m_pSettings->getResourceDir() + L"/asset-factories/";
    std::vector<std::wstring> vAssets = YCore::getListOfDirs(sPath);
    for (int i = 0; i < vAssets.size(); i++) {
        std::wstring sFactoryPath = sPath + vAssets[i];
        YLog::info(TAG, L"Try loading '" + sFactoryPath + L"'");
        std::wstring sError;
        std::wstring sAssetFactoryType;
        if (!pAssets->loadAssetFactory(sFactoryPath, sAssetFactoryType, sError)) {
            YLog::throw_err(TAG, sError);
        } else {
            YLog::info(TAG, L"Loaded and registered factory '" + vAssets[i] + L"' from " + sFactoryPath + L", factory type: " + sAssetFactoryType);
        }
    }

    m_pLoaderController->updateText(L"Generating background...");
    this->generateBackgrounds();
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Generation roads...");
    this->generateRoads(sDefaultPath);
    m_pLoaderController->addToProgressCurrent(1);


    m_pLoaderController->updateText(L"Generating buildings...");
    this->generateBuildings(sDefaultPath);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Load vegetations...");
    std::cout << "default/vegetations.json" << std::endl;
    YJson jsonDefaultVegetations(sDefaultPath + L"/vegetations.json");
    if (jsonDefaultVegetations.isParserFailed()) {
        return false;
    }
    this->loadVegetations(sDefaultPath, jsonDefaultVegetations[L"vegetations"]);
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Generating transports...");
    this->generateTransports();
    m_pLoaderController->addToProgressCurrent(1);

    // generate alien-berries
    m_pLoaderController->updateText(L"Generating alien-berries...");
    this->generateAlienBerries(1);
    m_pLoaderController->addToProgressCurrent(1);

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

    m_pGameState->setMinPoint(m_cfgDefaultMap->getMinPointMap());
    m_pGameState->setMaxPoint(m_cfgDefaultMap->getMaxPointMap());
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Generating clouds...");
    this->generateClouds();
    m_pLoaderController->updateText(L"Generating screen highlights...");
    this->generateScreenHighlights();
    m_pLoaderController->addToProgressCurrent(1);

    m_pLoaderController->updateText(L"Prepare panels...");
    m_pWindow->getRenderWindow()->addPanelsObject(
        new RenderLeftPanelInfo(
            m_pTextureLeftPanel,
            new RenderPlayerPower(m_pTexturePlayerPower0, m_pGameState->getAlienShipState()),
            5000
        )
    );
    m_pMinimap = pAssets->createAsset<YAssetMinimap>(L"minimap1");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pMinimap);


    // FPS text
    m_pFpsText = pAssets->createAsset<YAssetText>(L"text1");
    m_pFpsText->setOrderZ(5001);
    m_pFpsText->setAbsolutePosition(true);
    m_pFpsText->setPosition(m_pWindow->getWidth() - 1250, 20);
    m_pFpsText->setText(L"FPS: ...");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pFpsText);

    // score text
    m_pScoreText = pAssets->createAsset<YAssetText>(L"text1");
    m_pScoreText->setOrderZ(5001);
    m_pScoreText->setAbsolutePosition(true);
    m_pScoreText->setPosition(270, 20);
    m_pScoreText->setAnchor(YAssetTextAnchor::TOP_RIGHT);
    m_pScoreText->setText(L"Score: ");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pScoreText);

    m_pDialogHelp = pAssets->createAsset<YAssetDialogHelp>(L"dialog_help1");
    m_pDialogHelp->setOrderZ(6001);
    m_pWindow->getRenderWindow()->addPanelsObject(m_pDialogHelp);

    // coordinates of player
    m_pCoordText = pAssets->createAsset<YAssetText>(L"text1");
    m_pCoordText->setOrderZ(5001);
    m_pCoordText->setAbsolutePosition(true);
    m_pCoordText->setPosition(270, 50);
    m_pCoordText->setAnchor(YAssetTextAnchor::TOP_RIGHT);
    m_pCoordText->setText(L"x = ? y = ?");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pCoordText);

    m_pDialogGameOver = pAssets->createAsset<YAssetDialogHelp>(L"game-over-screen");
    m_pDialogGameOver->setOrderZ(6001);
    m_pWindow->getRenderWindow()->addPanelsObject(m_pDialogGameOver);

    // takeberry countdown
    m_pTakeBerryText = pAssets->createAsset<YAssetText>(L"text1");
    m_pTakeBerryText->setOrderZ(5001);
    m_pTakeBerryText->setAbsolutePosition(true);
    m_pTakeBerryText->setPosition(-200, -200);
    m_pTakeBerryText->setText(L"10");
    m_pWindow->getRenderWindow()->addPanelsObject(m_pTakeBerryText);

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

        // return from help satte
        if (getMainState() == MainState::GAME_HELP) {
            setMainState(MainState::GAME_ACTION);
            setPauseGame(false);
        }
    }

    if (pKeyboard->isF11()) {
        toggleFullscreen();
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

        // show help window
        if (pKeyboard->isF1()) {
            setMainState(MainState::GAME_HELP);
            setPauseGame(true);
            m_pDialogHelp->setShow(true);
        }
    } else if (getMainState() == MainState::GAME_HELP) {
        if (pKeyboard->isF1()) {
            m_pDialogHelp->setShow(false);
            setMainState(MainState::GAME_ACTION);
            setPauseGame(false);
        }
    } else if (getMainState() == MainState::GAME_OVER) {
        if(pKeyboard->isEnter()) {
            m_pGameState->getAlienShipState()->resetHealthPoints();
            resetScore();
            setMainState(MainState::GAME_ACTION);
            m_pDialogGameOver->setShow(false);
        }
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

    YPos p0 = m_pGameState->getAlienShipState()->getPosition();

    // calculate intersection rockets and player
    const std::vector<GameRocketState *> &vRockets = m_pWindow->getRenderWindow()->getRockets();
    // YLog::info(TAG, L"vRockets.size(): " + std::to_wstring(vRockets.size()));

    for (int i = 0; i < vRockets.size(); i++) {
        GameRocketState *pRocket = vRockets[i];
        if (pRocket->isExploded() || pRocket->hasDestroyed()) {
            continue;
        }
        YPos p1 = pRocket->getPosition();
        // distance
        // TODO optimize calculate distance here
        double nDistance = p1.getDistance(YPos(p0.getX(), p0.getY()));
        if (getMainState() == MainState::GAME_ACTION) {
            if (nDistance < 30.0) {
                pRocket->explode();
                m_pGameState->getAlienShipState()->rocketAttack(pRocket);
                YLog::info(TAG, L"Attacked");
                m_pScreenAttack->flash(1000, 10);
                m_pSoundController->playAttacked();
            }
        }
    }

    // intersection player bioplasm and rocket
    const std::vector<GameBioplastState *> &vBioplasts = m_pWindow->getRenderWindow()->getBioplasts();

    for (int i = 0; i < vRockets.size(); i++) {
        GameRocketState *pRocket = vRockets[i];
        if (pRocket->isExploded() || pRocket->hasDestroyed()) {
            continue;
        }
        YPos p1 = pRocket->getPosition();

        for (int b = 0; b < vBioplasts.size(); b++) {
            GameBioplastState *pBioplast = vBioplasts[b];
            YPos p2 = pBioplast->getPosition();
            // distance
            float nDistance = p1.getDistance(p2);
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
    const YPos &playerCoord = m_pGameState->getAlienShipState()->getPosition();
    std::wstring sCoordPlayer = L"X=" + std::to_wstring(playerCoord.getX())
            + L"\nY=" + std::to_wstring(playerCoord.getY());
    m_pCoordText->setText(sCoordPlayer);

    int nBerryIndex = this->findAlienBerryIndex(playerCoord);

    if (nBerryIndex != m_nCurrentTakeAlienBerry) {
        m_pTakeBerryText->setPosition(m_pWindow->getWidth() / 2 - 270, m_pWindow->getHeight() / 2);
        // YLog::info(TAG, L"nBerryIndex=" + std::to_wstring(nBerryIndex));
        m_nCurrentTakeAlienBerry = nBerryIndex;
        if (m_nCurrentTakeAlienBerry < 0) {
            m_pSoundController->stopTakeBerry();
            m_pTakeBerryText->hideText();
        } else {
            m_pSoundController->playTakeBerry();
            m_pTakeBerryText->showText();
            m_nTakeBerryStartTime = getCurrentTimeInMilliseconds();
            m_nTakeBerryCoundDown = 10 - (getCurrentTimeInMilliseconds() - m_nTakeBerryStartTime) / 1000;
        }
    }

    if (nBerryIndex >= 0 && nBerryIndex == m_nCurrentTakeAlienBerry) {
        m_nTakeBerryCoundDown = 10 - (getCurrentTimeInMilliseconds() - m_nTakeBerryStartTime) / 1000;
        m_pTakeBerryText->setText(std::to_wstring(m_nTakeBerryCoundDown));
        if (m_nTakeBerryCoundDown == 0) {
            m_pSoundController->stopTakeBerry();
            m_pTakeBerryText->hideText();
            m_nTakedPlayerBerries = m_nTakedPlayerBerries + 1;
            GameAlienShipState *pAlientShipState = m_pGameState->getAlienShipState();
            pAlientShipState->getShootingStrategyLogic()->onScoreChanged(m_nTakedPlayerBerries);
            YPos p = this->generateRandomPositionAlienBerry();
            // nBerryIndex
            m_vAlienBerriesStates[nBerryIndex]->updatePosition(p);
            this->generateAlienBerries(1);

        }
    }
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
void MainController::updateScore() {
    m_pScoreText->setText(L"Score: " + std::to_wstring(m_nTakedPlayerBerries));
}

void MainController::resetScore() {
    m_nTakedPlayerBerries = 0;
    GameAlienShipState *pAlientShipState = m_pGameState->getAlienShipState();
    pAlientShipState->getShootingStrategyLogic()->onScoreChanged(m_nTakedPlayerBerries);
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
    } else if (m_nCurrentState == MainState::GAME_HELP) {
        YLog::info(TAG, L"setMainState MainState::GAME_HELP");
    } else if (m_nCurrentState == MainState::GAME_ACTION) {
        YLog::info(TAG, L"setMainState MainState::WAITING_SPACE");
    } else if (m_nCurrentState == MainState::GAME_EXIT) {
        YLog::info(TAG, L"setMainState MainState::GAME_EXIT");
    } else if (m_nCurrentState == MainState::GAME_OVER) {
        YLog::info(TAG, L"setMainState MainState::GAME_OVER");
    } else {
        YLog::info(TAG, L"setMainState ???");
    }
}

bool MainController::isPauseGame() {
    return m_pGameState->isPauseGame();
}


void MainController::setPauseGame(bool bPause) {
    if (bPause) {
        m_pMainAiThread->pause();
        m_pGameState->setPauseGame(true);
    } else {
        m_pMainAiThread->unpause();
        m_pGameState->setPauseGame(false);
    }
}

void MainController::generateBackgrounds() {
    auto *pAssets = findYService<YAssetsService>();
    int nPaddingTextureCount = m_cfgDefaultMap->getBackgroundPaddingTextureCount();

    // TODO redesign - need add calls from fabric (it know weight and height of texture)
    auto *pBackgroundTile = pAssets->createAsset<YAssetBackground>(m_cfgDefaultMap->getBackgroundAssetId());
    int nTextureWidth = pBackgroundTile->getWidth();
    int nTextureHeight = pBackgroundTile->getHeight();
    delete pBackgroundTile;

    // it's need for fullscreen
    int nStartX = - nTextureWidth * nPaddingTextureCount;
    int nEndX = m_cfgDefaultMap->getWidth() + nTextureWidth * nPaddingTextureCount;
    int nStartY = - nTextureHeight * nPaddingTextureCount;
    int nEndY = m_cfgDefaultMap->getHeight() + nTextureHeight * nPaddingTextureCount;

     for (int x = nStartX; x <= nEndX; x += nTextureWidth) {
        for (int y = nStartY; y <= nEndY; y += nTextureHeight) {
            auto *pBackgroundTile0 = pAssets->createAsset<YAssetBackground>(m_cfgDefaultMap->getBackgroundAssetId());
            pBackgroundTile0->setAbsolutePosition(YPos(x, y));
            m_pWindow->getRenderWindow()->addGroundObject(pBackgroundTile0);
        }
    }
}

void MainController::generateClouds() {
    auto *pAssets = findYService<YAssetsService>();

    for (int i = 0; i < m_cfgDefaultMap->getMaxClouds(); i++) {
        int nX = std::rand() % m_cfgDefaultMap->getWidth();
        nX += m_cfgDefaultMap->getMinPointMap().getX();
        int nY = std::rand() % m_cfgDefaultMap->getHeight();
        nY += m_cfgDefaultMap->getMinPointMap().getY();

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

void MainController::generateScreenHighlights() {
    auto *pAssets = findYService<YAssetsService>();
    m_pScreenAttack = pAssets->createAsset<YAssetScreenBorderFlashHighlight>(L"screen_border_attach_highlight");
    m_pWindow->getRenderWindow()->addScreenEffectsObject(m_pScreenAttack);
}

void MainController::generateRoads(const std::wstring &sDefaultPath) {
    auto pAssets = findYService<YAssetsService>();

    static const std::wstring sRoadAssetId = L"road1";

    int nTextureWidth = 0;
    int nTextureHeight = 0;

    {
        auto *pRoad = pAssets->createAsset<YAssetRoad>(sRoadAssetId);
        nTextureWidth = pRoad->getFrameWeight();
        nTextureHeight = pRoad->getFrameHeight();
        delete pRoad;
    }
    int nRoadCeilW = m_cfgDefaultMap->getWidth() / nTextureWidth;
    int nRoadCeilH = m_cfgDefaultMap->getHeight() / nTextureHeight;

    int nGeneratedRoads = 0;
    Roads2DGenerator road2gen(nRoadCeilW, nRoadCeilH);
    float nDensity = 0.5;
    road2gen.generate(nDensity);

    std::vector<std::vector<std::string>> vRoads = road2gen.exportToTable();

    for (int y = 0; y < vRoads.size(); y++) {
        const std::vector<std::string> &vRow = vRoads[y];
        for (int x = 0; x < vRow.size(); x++) {
            std::wstring sRoadPart = YCore::s2ws(vRow[x]);
            if (sRoadPart == L"") {
                continue;
            }
            nGeneratedRoads++;
            int nX = y * nTextureWidth + nTextureWidth;
            int nY = x * nTextureHeight;
            m_pMap->addRoad(YRect(nX, nY, nX + nTextureWidth, nY + nTextureHeight));

            auto *pRoad = pAssets->createAsset<YAssetRoad>(sRoadAssetId);
            pRoad->setAbsolutePosition(YPos(nX, nY));
            pRoad->setRoadPart(sRoadPart);
            m_pWindow->getRenderWindow()->addRoadsObject(pRoad);
        }
    }
    if (nGeneratedRoads == 0) {
        YLog::throw_err(TAG, L"Could not generate roads....");
    }
    YLog::info(TAG, L"Done.");
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


void MainController::generateBuildings(const std::wstring &sDefaultPath) {
    YLog::info(TAG, L"generateBuildings....");
    auto pAssets = findYService<YAssetsService>();
    int nBuildings = 0;

    std::vector<std::wstring> vFactoriyIDs = pAssets->findFactoryIDsByFactoryType(L"building");
    for (int i = 0; i < vFactoriyIDs.size(); i++) {
        YLog::info(TAG, L"Found building " + vFactoriyIDs[i]);
    }
    int nInfinityProtect = 0;
    while (nBuildings < m_cfgDefaultMap->getMaxBuildings()) {
        int nX = (std::rand() % (m_cfgDefaultMap->getWidth() - 1000)) + 500;
        int nY = (std::rand() % (m_cfgDefaultMap->getHeight() - 1000)) + 500;
        std::wstring sFactoryId = vFactoriyIDs[std::rand() % vFactoriyIDs.size()];
        auto *pBuilding = pAssets->createAsset<YAssetBuilding>(sFactoryId);
        YRect rectBuilding(nX, nY, nX + pBuilding->getWidth(), nY + pBuilding->getHeight());
        if (!m_pMap->isFreeRect(rectBuilding)) {
            nInfinityProtect++;
            if (nInfinityProtect > 5000) {
                YLog::info(TAG, L"InfinityProtect generated buildings " + std::to_wstring(nBuildings) + L" from " + std::to_wstring(m_cfgDefaultMap->getMaxBuildings()));
                break;
            }
            delete pBuilding;
            continue; // next generation
        }
        m_pMap->addBuilding(rectBuilding);
        nInfinityProtect = 0;

        pBuilding->setAbsolutePosition(YPos(nX, nY));

        m_pWindow->getRenderWindow()->addBuildingsObject(pBuilding);
        nBuildings++;
    }

    YLog::info(TAG, L"generateBuildings... done");
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

void MainController::generateTransports() {
    auto pAssets = findYService<YAssetsService>();

    int nGenerated = 0;
    int nMaxGenerated = 100;

    while (nGenerated < nMaxGenerated) {
        int nX = std::rand() % m_cfgDefaultMap->getWidth();
        int nY = std::rand() % m_cfgDefaultMap->getHeight();
        if (m_pMap->canDriveToPoint(YPos(nX, nY))) {
            nGenerated++;
            GameTank0State *pTankState = new GameTank0State(YPos(nX,nY));
            AiTank0 *pAiTank0 = new AiTank0(pTankState);
            auto *pTank = pAssets->createAsset<YAssetTank>(L"tank1");
            pTank->setGameStateTank(pTankState);
            m_pWindow->getRenderWindow()->addTransportsObject(pTank);
            m_pMainAiThread->addAiObject(pAiTank0);
        }
    }
}

void MainController::generateAlienBerries(int nMaxGenerate) {
    auto pAssets = findYService<YAssetsService>();

    int nGenerated = 0;
    while (nGenerated < nMaxGenerate) {
        YPos p = generateRandomPositionAlienBerry();
        auto *pBerry = pAssets->createAsset<YAssetAlienBerry>(L"berry1");
        // TODO replace to YRect
        auto *pBerryState = new GameAlienBerryState(p, pBerry->getFrameWidth(), pBerry->getFrameHeight());
        pBerry->setState(pBerryState);
        m_vAlienBerriesStates.push_back(pBerryState);

        YLog::info(TAG, L"Berry. X=" + std::to_wstring(p.getX()) + L", Y=" + std::to_wstring(p.getY()));
        // TODO
        m_pWindow->getRenderWindow()->addVegetationObject(pBerry);

        // TODO need something else
        m_pMap->addAlienBerry(pBerryState);
        nGenerated++;
    }
}

YPos MainController::generateRandomPositionAlienBerry() {
    // TODO: don't generate over buildings and over roads
    // generate to free space
    int nX = std::rand() % (m_cfgDefaultMap->getWidth() - 200) + 100;
    int nY = std::rand() % (m_cfgDefaultMap->getHeight() - 200) + 100;
    return YPos(nX, nY);
}

int MainController::findAlienBerryIndex(const YPos &p) {
    int x = p.getX();
    int y = p.getY();
    for (int i = 0; i < m_vAlienBerriesStates.size(); i++) {
        if (m_vAlienBerriesStates[i]->hasPoint(x,y)) {
            return i;
        }
    }
    return -1;
}
