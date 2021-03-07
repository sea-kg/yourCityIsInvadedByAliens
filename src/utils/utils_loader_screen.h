#pragma once
#include <string>
#include "render.h"

class UtilsLoaderScreen {
    public:
        UtilsLoaderScreen(RenderWindow *pRenderWindow, GameState *pGameState);
        ~UtilsLoaderScreen();
        void init();
        void updateText(const std::string &sNewText);
        void draw();

    private:
        void addObject(RenderObject *pObject);
        GameState *m_pGameState;

        RenderWindow *m_pRenderWindow;
        SDL_Texture *m_pTextureLoaderBackground;
        std::vector<RenderObject *> m_vObjects;

        RenderAbsoluteTextBlock *m_pText;
};