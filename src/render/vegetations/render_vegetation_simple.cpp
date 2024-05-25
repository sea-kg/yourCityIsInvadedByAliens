#include "render_vegetation_simple.h"


// ---------------------------------------------------------------------
// RenderVegetationSimple

RenderVegetationSimple::RenderVegetationSimple(
    const YPos &pos,
    int nWidth,
    int nHeight,
    SDL_Texture* pTexture
)
: RenderObject(600) {
    m_pTexture = pTexture;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nWidth;
    m_currentFrame.h = m_nHeight;

    m_coordAbsolute = pos;
    // m_maxPos = pos + YPos(200,300);
}

void RenderVegetationSimple::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRender = m_coordAbsolute - state.getCoordLeftTop();
}

bool RenderVegetationSimple::canDraw(const GameState& state) {
    /*return
        m_minPos.isInsideRect(state.getWindowRect())
        || m_maxPos.isInsideRect(state.getWindowRect())
    ;*/
    return true;
}

void RenderVegetationSimple::draw(SDL_Renderer* renderer) {

    SDL_Rect dst;
    dst.x = m_coordRender.getX();
    dst.y = m_coordRender.getY();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
}