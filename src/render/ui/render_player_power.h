
#pragma once
#include "render.h"
#include "game_alien_ship_state.h"

class RenderPlayerPower : public RenderObject {

    public:
        RenderPlayerPower(
            SDL_Texture* tex,
            GameAlienShipState *pAlienShipState,
            int nPositionZ = 0
        );
        void updatePosition(YPos pos);
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        SDL_Texture* m_pTexture;
        YPos m_position;
        SDL_Rect m_currentFrame;
        GameAlienShipState *m_pAlienShipState;
};