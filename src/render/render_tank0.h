
#pragma once
#include "render.h"
#include "game_tank0_state.h"

class RenderTank0 : public RenderObject {

    public:
        RenderTank0(
            GameTank0State *pTank0State,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        long m_nPrevPosition;
        GameTank0State *m_pTank0State;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};