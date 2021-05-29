#pragma once
#include "render_window.h"

class RenderLine : public RenderObject {

    public:
        RenderLine(
            const CoordXY &p1,
            const CoordXY &p2,
            const RenderColor &color,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

        const CoordXY &getAbsoluteCoord1();
        const CoordXY &getAbsoluteCoord2();
        const CoordXY &getCoord1();
        const CoordXY &getCoord2();
        void updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2);

    private:
        CoordXY m_startCoord1;
        CoordXY m_startCoord2;
        CoordXY m_coord1;
        CoordXY m_coord2;
        RenderColor m_color;
};