#pragma once
#include "render_window.h"

class RenderVegetationSimple : public RenderObject {

    public:
        RenderVegetationSimple(
            const YPos &pos,
            int nWidth,
            int nHeight,
            SDL_Texture* pTexture
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
    private:
        YPos m_coordAbsolute;
        YPos m_coordRender;
        YPos m_minPos;
        YPos m_maxPos;
        // GameBuilding *m_pBuilding;
        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
        int m_nWidth;
        int m_nHeight;
};