
#pragma once
#include "render.h"
#include "render_player_power.h"

class RenderLeftPanelInfo : public RenderObject {

    public:
        RenderLeftPanelInfo(
            SDL_Texture* tex,
            RenderPlayerPower *pRenderPlayerPower,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        YPos m_coordCenter;
        YPos m_coordReal;
        long m_nPrevPosition;
        int m_nWindowWidth;
        int m_nWindowHeight;

        SDL_Rect m_currentFrameTop;
        SDL_Rect m_currentFrameMiddle;
        SDL_Rect m_currentFrameBottom;
        SDL_Texture* m_pTexture;

        RenderPlayerPower *m_pRenderPlayerPower;
};