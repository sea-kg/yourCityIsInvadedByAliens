#pragma once
#include "render_window.h"

#include "game_state.h"
#include "game_alien_ship_state.h"

#include <vector>
#include <string>


class RenderPlayerAlienShip : public RenderObject {

    public:
        RenderPlayerAlienShip(
            GameAlienShipState *pState,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        GameAlienShipState *m_pState;
        CoordXY m_coordPositionRendering;
        long m_nPrevPosition;
        long m_nSpeedAnimation;
        SDL_Rect currentFrame;
        SDL_Texture* m_pTexture;
};