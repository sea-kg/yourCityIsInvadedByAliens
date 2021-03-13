
#include "main_controller.h"
#include <iostream>
#include "render.h"
#include "render_tank0.h"
#include "render_left_panel_info.h"
#include "utils_loader_screen.h"
#include "json.hpp"
#include <fstream>


static const char *MY_COOL_MP3 = "res/sound/music/sea5kg - InvitedByAliens.mp3";

// MainController

MainController::MainController(const std::string &sWindowName) {
    m_sWindowName = sWindowName;
    m_nWindowWidth = 1280;
    m_nWindowHeight = 720;
    m_pRenderWindow = nullptr;
    m_nProgressBarStatus = 0;
    m_nProgressBarMax = 100;
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
    int flags = MIX_INIT_MP3;
    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    Mix_Music *music = Mix_LoadMUS(MY_COOL_MP3);
    Mix_PlayMusic(music, 1);

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

void MainController::loadGameDataWithProgressBar() {
    UtilsLoaderScreen loader(m_pRenderWindow, m_pGameState);
    loader.init();
    loader.updateText("Loading... textures");

    m_pTextureBackground = m_pRenderWindow->loadTexture("res/gfx/background.png");
    m_pTextureBuildingBlock = m_pRenderWindow->loadTexture("res/gfx/building-block.png");
    m_pTextureAlienShip1 = m_pRenderWindow->loadTexture("res/sprites/alien-ship.png");
    m_pTextureTank0 = m_pRenderWindow->loadTexture("res/sprites/tank0.png");
    m_pRenderWindow->loadTextureRocket("res/sprites/tank0-rocket.png");
    m_pTextureCursor = m_pRenderWindow->loadTexture("res/gfx/mouse-target.png");
    m_pTextureLeftPanel = m_pRenderWindow->loadTexture("res/textures/left-panel-info.png");
    m_pRenderWindow->loadTextureBioplast("res/sprites/alient-bioplast.png");

    loader.updateText("Loading... buildings");

    // load map from json
    std::ifstream ifs("./res/data.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    
    nlohmann::json jsonBuildings = jf["buildings"];
    CoordXY minPointMap(0,0);
    CoordXY maxPointMap(0,0);
    for (auto it = jsonBuildings.begin(); it != jsonBuildings.end(); ++it) {
        // std::cout << it.key() << " | " << it.value() << "\n";
        GameBuilding *pBuilding = new GameBuilding(it.value());
        m_pGameState->addBuilding(pBuilding);
        RenderBuilding2 *pRenderBuilding2 = new RenderBuilding2(pBuilding, m_pTextureBuildingBlock);
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
        m_pRenderWindow->addObject(pRenderBuilding2);
    }


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
            m_pRenderWindow->addObject(new RenderBackground(CoordXY(x, y), m_pTextureBackground, -10));        
        }
    }

    m_pGameState->setMinPoint(minPointMap);
    m_pGameState->setMaxPoint(maxPointMap);
    
    loader.updateText("Generating enimies...");
    this->generateTanks();

    m_pRenderWindow->addObject(
        new RenderLeftPanelInfo(m_pTextureLeftPanel, 5000)
    );

    // text
    m_pFpsText = new RenderAbsoluteTextBlock(CoordXY(m_nWindowWidth - 270, 20), "FPS: ...", 5001);
    m_pRenderWindow->addObject(m_pFpsText);

    // coordinates of player
    m_pCoordText = new RenderAbsoluteTextBlock(CoordXY(m_nWindowWidth - 270, 40), "x = ? y = ?", 5001);
    m_pRenderWindow->addObject(m_pCoordText);
}

bool MainController::isKeyboardUp(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        keyboard_state_array[SDL_SCANCODE_UP]
        && !keyboard_state_array[SDL_SCANCODE_LEFT]
        && !keyboard_state_array[SDL_SCANCODE_RIGHT]
        && !keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        keyboard_state_array[SDL_SCANCODE_W]
        && !keyboard_state_array[SDL_SCANCODE_A]
        && !keyboard_state_array[SDL_SCANCODE_D]
        && !keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardUpLeft(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        keyboard_state_array[SDL_SCANCODE_UP]
        && keyboard_state_array[SDL_SCANCODE_LEFT]
        && !keyboard_state_array[SDL_SCANCODE_RIGHT]
        && !keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        keyboard_state_array[SDL_SCANCODE_W]
        && keyboard_state_array[SDL_SCANCODE_A]
        && !keyboard_state_array[SDL_SCANCODE_D]
        && !keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardUpRight(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        keyboard_state_array[SDL_SCANCODE_UP]
        && !keyboard_state_array[SDL_SCANCODE_LEFT]
        && keyboard_state_array[SDL_SCANCODE_RIGHT]
        && !keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        keyboard_state_array[SDL_SCANCODE_W]
        && !keyboard_state_array[SDL_SCANCODE_A]
        && keyboard_state_array[SDL_SCANCODE_D]
        && !keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardDown(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        !keyboard_state_array[SDL_SCANCODE_UP]
        && !keyboard_state_array[SDL_SCANCODE_LEFT]
        && !keyboard_state_array[SDL_SCANCODE_RIGHT]
        && keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !keyboard_state_array[SDL_SCANCODE_W]
        && !keyboard_state_array[SDL_SCANCODE_A]
        && !keyboard_state_array[SDL_SCANCODE_D]
        && keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardDownLeft(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        !keyboard_state_array[SDL_SCANCODE_UP]
        && keyboard_state_array[SDL_SCANCODE_LEFT]
        && !keyboard_state_array[SDL_SCANCODE_RIGHT]
        && keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !keyboard_state_array[SDL_SCANCODE_W]
        && keyboard_state_array[SDL_SCANCODE_A]
        && !keyboard_state_array[SDL_SCANCODE_D]
        && keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardDownRight(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        !keyboard_state_array[SDL_SCANCODE_UP]
        && !keyboard_state_array[SDL_SCANCODE_LEFT]
        && keyboard_state_array[SDL_SCANCODE_RIGHT]
        && keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !keyboard_state_array[SDL_SCANCODE_W]
        && !keyboard_state_array[SDL_SCANCODE_A]
        && keyboard_state_array[SDL_SCANCODE_D]
        && keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardLeft(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        !keyboard_state_array[SDL_SCANCODE_UP]
        && keyboard_state_array[SDL_SCANCODE_LEFT]
        && !keyboard_state_array[SDL_SCANCODE_RIGHT]
        && !keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !keyboard_state_array[SDL_SCANCODE_W]
        && keyboard_state_array[SDL_SCANCODE_A]
        && !keyboard_state_array[SDL_SCANCODE_D]
        && !keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardRight(const Uint8 *keyboard_state_array) {
    bool bArrowUp =
        !keyboard_state_array[SDL_SCANCODE_UP]
        && !keyboard_state_array[SDL_SCANCODE_LEFT]
        && keyboard_state_array[SDL_SCANCODE_RIGHT]
        && !keyboard_state_array[SDL_SCANCODE_DOWN];
    bool bWasdUp = 
        !keyboard_state_array[SDL_SCANCODE_W]
        && !keyboard_state_array[SDL_SCANCODE_A]
        && keyboard_state_array[SDL_SCANCODE_D]
        && !keyboard_state_array[SDL_SCANCODE_S];
    return bArrowUp || bWasdUp;
}

bool MainController::isKeyboardF1(const Uint8 *keyboard_state_array) {
    return keyboard_state_array[SDL_SCANCODE_F12];
}

bool MainController::isKeyboardF12(const Uint8 *keyboard_state_array) {
    return keyboard_state_array[SDL_SCANCODE_F12];
}

bool MainController::isKeyboardSpace(const Uint8 *keyboard_state_array) {
    return keyboard_state_array[SDL_SCANCODE_SPACE];
}

void MainController::updatePlayerCoord() {
    std::string sCoordPlayer = "X=" + std::to_string(m_pGameState->getCoordLeftTop().x() + m_nWindowWidth/2)
            + " Y=" + std::to_string(m_pGameState->getCoordLeftTop().y() + m_nWindowHeight/2);
    m_pCoordText->updateText(sCoordPlayer);
}

void MainController::updateFpsValue(int nFps) {
    m_pFpsText->updateText("FPS: ~" + std::to_string(nFps));
    std::cout << "FPS: ~" << nFps << std::endl;
}

void MainController::generateTanks() {
     m_pRenderWindow->addObject(new RenderTank0(
        new GameTank0State(CoordXY(100,100)),
        m_pTextureTank0,
        1000
    ));

    m_pRenderWindow->addObject(new RenderTank0(
        new GameTank0State(CoordXY(100,200)),
        m_pTextureTank0,
        1000
    ));

    m_pRenderWindow->addObject(new RenderTank0(
        new GameTank0State(CoordXY(200,100)),
        m_pTextureTank0,
        1000
    ));

    m_pRenderWindow->addObject(new RenderTank0(
        new GameTank0State(CoordXY(200,200)),
        m_pTextureTank0,
        1000
    ));
}


