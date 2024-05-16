#pragma once
#include "render_window.h"

#include "game_state.h"
#include "ishooting_strategy.h"
#include "game_alien_ship_state.h"
#include "shooting_strategy_logic.h"

#include <vector>
#include <string>
#include "yjson.h"

class RenderAlienShip0 : public RenderObject {

    public:
        RenderAlienShip0(
            GameAlienShipState *pState,
            const YJson &jsonAlienShip,
            bool bShadow,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        std::wstring TAG;
        GameAlienShipState *m_pState;
        CoordXY m_coordPositionRendering;
        int m_nFrameWidth;
        int m_nFrameHeight;
        int m_nFramesCount;
        bool m_bFrameMoveDirectionVertical;
        bool m_bShadow;
        long m_nPrevPosition;
        long m_nSpeedAnimation;
        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};