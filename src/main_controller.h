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
        void modifyObjects();

        // keyboard - TODO redesign keyboard class
        bool isKeyboardUp(const Uint8 *keyboard_state_array);
        bool isKeyboardUpLeft(const Uint8 *keyboard_state_array);
        bool isKeyboardUpRight(const Uint8 *keyboard_state_array);
        bool isKeyboardDown(const Uint8 *keyboard_state_array);
        bool isKeyboardDownLeft(const Uint8 *keyboard_state_array);
        bool isKeyboardDownRight(const Uint8 *keyboard_state_array);
        bool isKeyboardLeft(const Uint8 *keyboard_state_array);
        bool isKeyboardRight(const Uint8 *keyboard_state_array);
        
        bool isKeyboardF1(const Uint8 *keyboard_state_array);
        bool isKeyboardF12(const Uint8 *keyboard_state_array);
        bool isKeyboardSpace(const Uint8 *keyboard_state_array);
            

        void updatePlayerCoord();
        void updateFpsValue(int nFps);
        GameAlienShipState *getGameAlienShipState();

    private:
        std::string TAG;

        void generateTanks();

        std::string m_sWindowName;
        std::string m_sResourceDir;
        CoordXY m_minPointMap;
        CoordXY m_maxPointMap;
        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nProgressBarStatus;
        int m_nProgressBarMax;
        RenderWindow *m_pRenderWindow;
        GameState *m_pGameState;
        MainAiThread *m_pMainAiThread;

        SDL_Texture* m_pTextureBackground;
        SDL_Texture* m_pTextureAlienShip1;
        SDL_Texture* m_pTextureTank0;
        SDL_Texture* m_pTextureRocket;
        SDL_Texture* m_pTextureCursor;
        SDL_Texture* m_pTextureLeftPanel;
        std::map<std::string, SDL_Texture*> m_mapBuildingsTextures;

        RenderAbsoluteTextBlock *m_pCoordText;
        RenderAbsoluteTextBlock *m_pFpsText;
        GameAlienShipState *m_pAlientShipState;
};