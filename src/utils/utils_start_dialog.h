#pragma once
#include <string>
#include "render.h"
#include "render_button_music_on_off.h"
#include "render_button_simple.h"


class UtilsStartDialog {
    public:
        UtilsStartDialog(
            const std::string &sResourceDir,
            RenderWindow *pRenderWindow,
            GameState *pGameState
        );
        ~UtilsStartDialog();
        void init();
        void draw();
        bool start();

    private:
        void addObject(RenderObject *pObject);
        std::string m_sResourceDir;
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