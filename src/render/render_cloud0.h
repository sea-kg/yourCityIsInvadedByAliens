
#pragma once
#include "render.h"
#include "game_cloud0_state.h"

class RenderCloud0 : public RenderObject {

    public:
        RenderCloud0(
            GameCloud0State *pCloud0State,
            SDL_Texture* tex,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        long m_nPrevPosition;
        GameCloud0State *m_pCloud0State;
        YRect m_rectRegionRender;
        
        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};