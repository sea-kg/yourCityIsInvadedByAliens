#pragma once
#include "render_line.h"

class RenderTriangle : public RenderObject {

    public:
        RenderTriangle(
            const YPos &p1,
            const YPos &p2,
            const YPos &p3,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        RenderColor m_color;
};