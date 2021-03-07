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
        
        // keyboard
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

    private:
        void generateTanks();

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