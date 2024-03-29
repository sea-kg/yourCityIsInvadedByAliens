#pragma once
#include <string>
#include "render_ui.h"

class UtilsStartDialog {
    public:
        UtilsStartDialog(
            const std::wstring &sResourceDir,
            RenderWindow *pRenderWindow,
            GameState *pGameState
        );
        ~UtilsStartDialog();
        void init();
        void draw();
        bool start();

    private:
        void addObject(RenderObject *pObject);
        std::wstring m_sResourceDir;
        GameState *m_pGameState;
        CoordXY m_posTopLeftMusicOnOff;
        CoordXY m_posBottomRightMusicOnOff;
        CoordXY m_posButtonStartTopLeft;
        CoordXY m_posButtonStartBottomRight;

        CoordXY m_mousePosition;
        RenderWindow *m_pRenderWindow;
        RenderButtonMusicOnOff *m_pRenderButtonMusicOnOff;
        RenderButtonSimple *m_pRenderButtonStart;
        SDL_Texture *m_pTextureDialogBackground;
        std::vector<RenderObject *> m_vObjects;
        RenderMouse *m_pMouseCursor;
};