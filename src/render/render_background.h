#pragma once
#include "render_window.h"

#include <vector>
#include <string>
#include <iostream>

class RenderBackground : public RenderObject {

    public:
        RenderBackground(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordPos;
        YRect m_rectRegionPos;
        CoordXY m_coordRender;
        YRect m_rectRegionRender;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};
