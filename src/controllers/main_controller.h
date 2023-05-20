#pragma once
#include <string>
#include "render_window.h"
#include "render.h"
#include "main_ai_thread.h"
#include "game_alien_ship_state.h"
#include "game_alien_berry_state.h"
#include "sound_controller.h"
#include "loader_controller.h"
#include "settings_yservice.h"
#include "window_yservice.h"

enum class MainState {
    LOADING,
    WAITING_SPACE,
    GAME_HELP,
    GAME_ACTION,
    GAME_OVER,
    GAME_EXIT
};

class MainController {
    public:
        MainController();
        ~MainController();

        // std::wstring getResourceDir();
        bool init();
        bool initSoundController();
        int startUI();
        void startGameLogicThread();
        void runGameLogicThread();

        GameState *getGameState();
        CoordXY getCoordCenter();
        bool loadGameDataWithProgressBar();
        void deinitLoaderController();
        bool showStartDialog();
        void startAllThreads();
        void handleKeyboardCommand(YKeyboard *pKeyboard);
        // window
        bool isFullscreen();
        void toggleFullscreen();
        void clearWindow();
        void modifyObjects();
        void drawObjects();

        void updatePlayerCoord();
        void startFpsCounting();
        void updateFps();
        void updateFpsValue(int nFps);
        void updateScore();
        void resetScore();

        SoundController *getSoundController();

        MainState getMainState();
        void setMainState(const MainState &newMainState);

        bool isPauseGame();
        void setPauseGame(bool bPause);

    private:
        std::wstring TAG;
        SettingsYService *m_pSettings;
        WindowYService *m_pWindow;
        MapYService *m_pMap;

        void loadBackgrounds(
            const std::wstring &sDefaultPath,
            const YJsonObject &jsonBackground
        );
        void generateBackground(
            SDL_Texture* pTextureBackground,
            int nTextureWidth,
            int nTextureHeight,
            const CoordXY &startXY,
            const CoordXY &endXY
        );
        void generateTanks();
        void generateClouds();
        void generateScreenHighlights();

        void generateRoads(const std::wstring &sDefaultPath);

        void loadAlienShip(
            const std::wstring &sDefaultPath
        );

        void loadBuildings(
            const std::wstring &sDefaultPath,
            const YJsonObject &jsonRoads
        );

        void loadVegetations(
            const std::wstring &sDefaultPath,
            const YJsonObject &jsonRoads
        );

        void generateTransports();
        void generateAlienBerries(int nMaxGenerate);
        YPos generateRandomPositionAlienBerry();
        int findAlienBerryIndex(int x, int y);

        // TODO move to MapYService
        CoordXY m_minPointMap;
        CoordXY m_maxPointMap;
        int m_nMapWidth;
        int m_nMapHeight;
        int m_nMaxClouds;
        std::wstring m_sMapName;

        int m_nProgressBarStatus;
        int m_nProgressBarMax;

        GameState *m_pGameState;
        MainAiThread *m_pMainAiThread;

        SDL_Texture* m_pTextureCursor;
        SDL_Texture* m_pTextureLeftPanel;
        SDL_Texture* m_pTexturePlayerPower0;
        std::map<std::wstring, SDL_Texture*> m_mapBuildingsTextures;

        YAssetText *m_pCoordText;
        YAssetText *m_pFpsText;
        YAssetText *m_pScoreText;
        YAssetDialogHelp *m_pDialogHelp;
        YAssetDialogHelp *m_pDialogGameOver;

        // TODO mustbe chnaged from GameState?
        YAssetScreenBorderFlashHighlight *m_pScreenAttack;
        YAssetMinimap *m_pMinimap;

        long m_nFpsNumberOfFrames;
        long m_nFpsStartTime;
        long m_nFpsElapsed;

        SoundController *m_pSoundController;
        LoaderController *m_pLoaderController;
        MainState m_nCurrentState;

        bool m_bGameLogicThreadStop;
        std::thread *m_pGameLogicThread;
        std::mutex m_mutexGameLogicThread;

        // alien berries
        std::vector<GameAlienBerryState *> m_vAlienBerriesStates;
        int m_nCurrentTakeAlienBerry;
        long m_nTakeBerryStartTime;
        long m_nTakeBerryCoundDown;
        YAssetText *m_pTakeBerryText;
        int m_nTakedPlayerBerries;
};