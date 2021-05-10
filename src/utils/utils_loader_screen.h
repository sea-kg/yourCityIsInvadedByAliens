#pragma once
#include <string>
#include "render.h"
#include "render_bootscreen_progressbar.h"

class UtilsLoaderScreen {
    public:
        UtilsLoaderScreen(
            const std::string &sResourceDir,
            RenderWindow *pRenderWindow,
            GameState *pGameState
        );
        ~UtilsLoaderScreen();
        void init();
        void updateText(const std::string &sNewText);
        void setProgressMax(int nVal);
        void setProgressCurrent(int nVal);
        void addToProgressMax(int nVal);
        void addToProgressCurrent(int nVal);
        void draw();

    private:
        void addObject(RenderObject *pObject);
        std::string m_sResourceDir;
        GameState *m_pGameState;

        RenderWindow *m_pRenderWindow;
        SDL_Texture *m_pTextureLoaderBackground;
        SDL_Texture *m_pTextureLogoBig;
        SDL_Texture *m_pTextureProgressBar;
        std::vector<RenderObject *> m_vObjects;

        RenderAbsoluteTextBlock *m_pText;
        RenderBootScreenProgressBar *m_pProgressBar;
        int m_nProgressCurrent;
        int m_nProgressMax;
};