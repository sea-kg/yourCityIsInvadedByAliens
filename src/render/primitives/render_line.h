#pragma once
#include "render_window.h"

class RenderLine : public RenderObject {

    public:
        RenderLine(
            const YPos &p1,
            const YPos &p2,
            const RenderColor &color,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

        const YPos &getAbsoluteCoord1();
        const YPos &getAbsoluteCoord2();
        const YPos &getCoord1();
        const YPos &getCoord2();
        void updateAbsoluteCoords(const YPos &p1, const YPos &p2);

    private:
        YPos m_startCoord1;
        YPos m_startCoord2;
        YPos m_coord1;
        YPos m_coord2;
        RenderColor m_color;
};