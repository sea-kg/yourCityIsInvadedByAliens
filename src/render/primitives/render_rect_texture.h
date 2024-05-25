#pragma once
#include "render_window.h"

class RenderRectTexture : public RenderObject {

    public:
        RenderRectTexture(
            const YPos &p0,
            SDL_Texture* tex,
            int nTextureWidth,
            int nTextureHeight,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        YPos m_coordPos;
        YPos m_coordPosEnd;
        YPos m_coordRender;
        YPos m_coordRenderEnd;

        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};