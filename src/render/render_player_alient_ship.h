#pragma once
#include "render_window.h"

#include "game_state.h"

#include <vector>
#include <string>


class RenderPlayerAlienShip : public RenderObject {

    public:
        RenderPlayerAlienShip(
            const CoordXY &p0,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;
        long m_nPrevPosition;
        long m_nSpeedAnimation;

        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};