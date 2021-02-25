#pragma once
#include <string>
#include "render_window.h"

class MainController {
    public:
        MainController(const std::string &sWindowName);
        bool initSDL2();
        bool initRenderWindow();
        RenderWindow *getWindow();
        SDL_Texture* loadTexture(const char* p_filePath);

    private:
        std::string m_sWindowName;
        int m_nWindowWidth;
        int m_nWindowHeight;
        RenderWindow *m_pRenderWindow;
};