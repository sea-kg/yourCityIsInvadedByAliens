#pragma once
#include <string>
#include "render_window.h"

class MainController {
    public:
        MainController(const std::string &sWindowName);
        bool initSDL2();
        bool initRenderWindow();
        RenderWindow *getWindow();
        GameState *getGameState();
        CoordXY getCoordCenter();
        void loadGameDataWithProgressBar();

    private:
        std::string m_sWindowName;
        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nProgressBarStatus;
        int m_nProgressBarMax;
        RenderWindow *m_pRenderWindow;
        GameState *m_pGameState;

        SDL_Texture* m_pTextureBackground;
        SDL_Texture* m_pTextureBuildingBlock;
        SDL_Texture* m_pTextureAlienShip1;
        SDL_Texture* m_pTextureTank0;
        SDL_Texture* m_pTextureRocket;
        SDL_Texture* m_pTextureCursor;
};