
#pragma once
#include "render.h"
#include "game_rocket_state.h"

class RenderRocket : public RenderObject {

    public:
        RenderRocket(
            GameRocketState *pTank0State,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual ~RenderRocket();

        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;
        long m_nPrevPosition;
        int m_nLifeTime;
        GameRocketState *m_pRocketState;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};