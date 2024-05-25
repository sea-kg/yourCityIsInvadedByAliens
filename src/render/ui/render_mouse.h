#pragma once
#include "render_window.h"

class RenderMouse : public RenderObject {

    public:
        RenderMouse(
            const YPos &p1,
            SDL_Texture* pTextureCursorTarget,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateCoord(const YPos &p0);

    private:
        int m_nCursorType;
        YPos m_p1;

        SDL_Texture* m_pTextureCursorTarget;
        SDL_Rect m_currentFrame;
};
