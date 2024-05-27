#include "render_line.h"

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(const YPos &p1, const YPos &p2, const RenderColor &color, int nPositionZ)
: RenderObject(nPositionZ), m_color(color) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coord1 = m_startCoord1 - state.getCoordLeftTop();
    m_coord2 = m_startCoord2 - state.getCoordLeftTop();
}

bool RenderLine::canDraw(const GameState& state) {
    return
        m_coord1.isInsideRect(state.getWindowRect())
        || m_coord2.isInsideRect(state.getWindowRect())
    ;
}

void RenderLine::draw(SDL_Renderer* renderer) {
    m_color.changeRenderColor(renderer);
    SDL_RenderDrawLine(renderer, m_coord1.getX(), m_coord1.getY(), m_coord2.getX(), m_coord2.getY());
}

const YPos &RenderLine::getAbsoluteCoord1() {
    return m_startCoord1;
}

const YPos &RenderLine::getAbsoluteCoord2() {
    return m_startCoord2;
}

const YPos &RenderLine::getCoord1() {
    return m_coord1;
}

const YPos &RenderLine::getCoord2() {
    return m_coord2;
}

void RenderLine::updateAbsoluteCoords(const YPos &p1, const YPos &p2) {
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}