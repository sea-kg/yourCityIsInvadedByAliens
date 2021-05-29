#include "render_triangle.h"

// ---------------------------------------------------------------------
// RenderTriangle

RenderTriangle::RenderTriangle(
    const CoordXY &p1,
    const CoordXY &p2,
    const CoordXY &p3,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_color(255,255,255,255),
    m_line1(p1,p2, m_color),
    m_line2(p2,p3, m_color),
    m_line3(p3,p1, m_color)
{
    // nothing
}

void RenderTriangle::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_line1.modify(state, pRenderWindow);
    m_line2.modify(state, pRenderWindow);
    m_line3.modify(state, pRenderWindow);
}

bool RenderTriangle::canDraw(const GameState& state) {
    return
        m_line1.canDraw(state)
        && m_line2.canDraw(state)
        && m_line3.canDraw(state)
    ;
}

void RenderTriangle::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);
}