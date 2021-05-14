
#pragma once
#include "render.h"

class RenderBootScreenProgressBar : public RenderObject {

    public:
        RenderBootScreenProgressBar(
            SDL_Texture* tex,
            CoordXY pos,
            int nPositionZ = 0
        );
        void updateProgress(int nProgressMax, int nProgressCurrent);
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        SDL_Texture* m_pTexture;
        CoordXY m_position;
        SDL_Rect m_currentFrame;
        SDL_Rect m_currentFrame2;
        int m_nCurProgress;
        int m_nMaxProgress;
};