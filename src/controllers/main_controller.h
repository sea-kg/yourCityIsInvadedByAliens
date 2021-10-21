#pragma once
#include <string>
#include "render_window.h"
#include "render.h"
#include "main_ai_thread.h"
#include "game_alien_ship_state.h"
#include "sound_controller.h"
#include "loader_controller.h"
#include "settings_yservice.h"
#include "window_yservice.h"

enum class MainState {
    LOADING,
    WAITING_SPACE,
    GAME_ACTION,
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

        SoundController *getSoundController();

        MainState getMainState();
        void setMainState(const MainState &newMainState);

    private:
        std::wstring TAG;
        SettingsYService *m_pSettings;
        WindowYService *m_pWindow;

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
        
        void loadRoads(
            const std::wstring &sDefaultPath,
            const YJsonObject &jsonRoads
        );

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

        void loadTransports(
            const std::wstring &sDefaultPath,
            const YJsonObject &jsonRoads
        );
        
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
        std::vector<SDL_Texture*> m_vTexturesClouds;
        SDL_Texture* m_pTextureCursor;
        SDL_Texture* m_pTextureLeftPanel;
        SDL_Texture* m_pTexturePlayerPower0;
        std::map<std::wstring, SDL_Texture*> m_mapBuildingsTextures;

        RenderAbsoluteTextBlock *m_pCoordText;
        RenderAbsoluteTextBlock *m_pFpsText;

        long m_nFpsNumberOfFrames;
        long m_nFpsStartTime;
        long m_nFpsElapsed;

        SoundController *m_pSoundController;
        LoaderController *m_pLoaderController;
        MainState m_nCurrentState;

        bool m_bGameLogicThreadStop;
        std::thread *m_pGameLogicThread;
        std::mutex m_mutexGameLogicThread;
};