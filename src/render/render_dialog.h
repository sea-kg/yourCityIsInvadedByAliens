
#pragma once
#include "render.h"

class RenderDialog : public RenderObject {

    public:
        RenderDialog(
            SDL_Texture* tex,
            int nWidth,
            int nHeight,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordCenter;
        CoordXY m_coordReal;
        int m_nWindowWidth;
        int m_nWindowHeight;
        int m_nDialogWidth;
        int m_nDialogHeight;

        SDL_Rect m_currentFrameTopLeft;
        SDL_Rect m_currentFrameTopMiddle;
        SDL_Rect m_currentFrameTopRight;
        SDL_Rect m_currentFrameLeftMiddle;
        SDL_Rect m_currentFrameCenter;
        SDL_Rect m_currentFrameRightMiddle;
        SDL_Rect m_currentFrameBottomLeft;
        SDL_Rect m_currentFrameBottomMiddle;
        SDL_Rect m_currentFrameBottomRight;
        SDL_Texture* m_pTexture;
};