#include "render_mouse.h"

// ---------------------------------------------------------------------
// RenderMouse

RenderMouse::RenderMouse(
    const YPos &p1,
    SDL_Texture* pTextureCursorTarget,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_p1(p1),
    m_nCursorType(0)
{
    m_pTextureCursorTarget = pTextureCursorTarget;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 24;
    m_currentFrame.h = 24;

    this->updateCoord(p1);
}

void RenderMouse::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // m_pLineMoveble1->modify(state);
    // m_pLineMoveble2->modify(state);
}

bool RenderMouse::canDraw(const GameState& state) {
    return true;
}

void RenderMouse::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_p1.getX() - 12;
    dst.y = m_p1.getY() - 12;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;
    SDL_RenderCopy(renderer, m_pTextureCursorTarget, &m_currentFrame, &dst);
}

void RenderMouse::updateCoord(const YPos &p0) {
    m_p1 = p0;
}
