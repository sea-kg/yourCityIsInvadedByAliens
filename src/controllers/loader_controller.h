#pragma once
#include <string>
#include "render_ui.h"
#include <yasset_text.h>

class LoaderController {
    public:
        LoaderController(
            const std::wstring &sResourceDir,
            RenderWindow *pRenderWindow,
            GameState *pGameState
        );
        void init();
        void deinit();
        void updateText(const std::wstring &sNewText);
        void setProgressMax(int nVal);
        void setProgressCurrent(int nVal);
        void addToProgressMax(int nVal);
        void addToProgressCurrent(int nVal);
        void draw();

    private:
        void addObject(RenderObject *pObject);
        std::wstring m_sResourceDir;
        GameState *m_pGameState;

        RenderWindow *m_pRenderWindow;
        SDL_Texture *m_pTextureLoaderBackground;
        SDL_Texture *m_pTextureLogoBig;
        SDL_Texture *m_pTextureProgressBar;
        std::vector<RenderObject *> m_vObjects;

        YAssetText *m_pAssetText;
        RenderBootScreenProgressBar *m_pProgressBar;
        int m_nProgressCurrent;
        int m_nProgressMax;
};