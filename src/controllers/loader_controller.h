#pragma once
#include <string>
#include "render_ui.h"
#include <yassets_ui.h>

class LoaderController {
    public:
        LoaderController(
            RenderWindow *pRenderWindow,
            GameState *pGameState
        );
        void init();
        void deinit();
        void updateText(const std::wstring &sNewText);
        void setProgressMax(int nVal);
        void setProgressCurrent(int nVal);
        void addToProgressCurrent(int nVal);
        void draw();
        void addObject(RenderObject* pObject);

    private:
        std::wstring TAG; 
        GameState *m_pGameState;

        RenderWindow *m_pRenderWindow;
        std::vector<RenderObject *> m_vObjects;

        YAssetText *m_pAssetText;
        YAssetProgressBar *m_pAssetProgressBar;
        int m_nProgressCurrent;
        int m_nProgressMax;
};