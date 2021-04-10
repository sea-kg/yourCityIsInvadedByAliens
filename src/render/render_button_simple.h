
#pragma once
#include "render.h"

class RenderButtonSimple : public RenderObject {

    public:
        RenderButtonSimple(
            SDL_Texture* tex,
            CoordXY pos,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void setAnimate(bool bAnimate);

    private:
        SDL_Texture* m_pTexture;
        CoordXY m_position;
        int m_nTextureWidth;
        int m_nTextureHeight;
        bool m_bAnimate;

        SDL_Rect m_currentFrame;
};