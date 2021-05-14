
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
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        YPos m_coordRender;
        YPos m_coordRenderEnd;
        YPos m_size;
        long m_nPrevPosition;
        int m_nLifeTime;
        int m_nMaxLifeTime;
        long m_nSpeedAnimation;
        GameRocketState *m_pRocketState;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};