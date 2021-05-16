
#pragma once
#include "render.h"
#include "game_bioplast_state.h"

class RenderBioplast : public RenderObject {

    public:
        RenderBioplast(
            GameBioplastState *pTank0State,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual ~RenderBioplast();

        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordPos;
        CoordXY m_coordPosEnd;
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        long m_nPrevPosition;
        int m_nLifeTime;
        int m_nMaxLifeTime;
        long m_nSpeedAnimation;
        GameBioplastState *m_pBioplastState;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};