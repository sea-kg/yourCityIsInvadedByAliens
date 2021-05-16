#pragma once
#include <string>
#include "render_window.h"
#include "render.h"
#include "main_ai_thread.h"
#include "game_alien_ship_state.h"

class MainController {
    public:
        MainController(const std::string &sWindowName);
        bool findResourceDir();
        std::string getResourceDir();
        bool initSDL2();
        bool initRenderWindow();
        RenderWindow *getWindow();
        GameState *getGameState();
        CoordXY getCoordCenter();
        bool loadGameDataWithProgressBar();
        bool showStartDialog();
        void startAllThreads();

        // window
        bool isFullscreen();
        void toggleFullscreen();
        void clearWindow();
        void modifyObjects();
        void drawObjects();

        void updatePlayerCoord();
        void updateFpsValue(int nFps);
        GameAlienShipState *getGameAlienShipState();

    private:
        std::string TAG;

        void loadBackgrounds(
            const std::string &sDefaultPath,
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
            const std::string &sDefaultPath,
            const YJsonObject &jsonRoads
        );

        void loadAlienShip(
            const std::string &sDefaultPath
        );

        std::string m_sWindowName;
        std::string m_sResourceDir;
        CoordXY m_minPointMap;
        CoordXY m_maxPointMap;
        CoordXY m_playerStartPosition;
        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nMapWidth;
        int m_nMapHeight;
        int m_nMaxClouds;
        std::string m_sMapName;

        int m_nProgressBarStatus;
        int m_nProgressBarMax;
        RenderWindow *m_pRenderWindow;
        GameState *m_pGameState;
        MainAiThread *m_pMainAiThread;
        SDL_Texture* m_pTextureCloud0;
        SDL_Texture* m_pTextureRoad0;
        SDL_Texture* m_pTextureTank0;
        SDL_Texture* m_pTextureRocket;
        SDL_Texture* m_pTextureCursor;
        SDL_Texture* m_pTextureLeftPanel;
        SDL_Texture* m_pTexturePlayerPower0;
        std::map<std::string, SDL_Texture*> m_mapBuildingsTextures;

        RenderAbsoluteTextBlock *m_pCoordText;
        RenderAbsoluteTextBlock *m_pFpsText;
        GameAlienShipState *m_pAlientShipState;
};