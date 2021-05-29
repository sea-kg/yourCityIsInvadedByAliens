#pragma once
#include "render_window.h"

class RenderRectTexture : public RenderObject {

    public:
        RenderRectTexture(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nTextureWidth, 
            int nTextureHeight,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordPos;
        CoordXY m_coordPosEnd;
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;

        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};