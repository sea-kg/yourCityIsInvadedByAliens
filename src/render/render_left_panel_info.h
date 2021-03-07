
#pragma once
#include "render.h"
#include "game_tank0_state.h"
#include "ai_tank0.h"

class RenderLeftPanelInfo : public RenderObject {

    public:
        RenderLeftPanelInfo(
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;
        long m_nPrevPosition;
        int m_nWindowWidth;
        int m_nWindowHeight;

        SDL_Rect m_currentFrameTop;
        SDL_Rect m_currentFrameMiddle;
        SDL_Rect m_currentFrameBottom;
        SDL_Texture* m_pTexture;
};