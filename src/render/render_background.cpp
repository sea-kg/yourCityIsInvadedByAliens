#include "render_background.h"

// ---------------------------------------------------------------------
// RenderBackground

RenderBackground::RenderBackground(const CoordXY &p0, SDL_Texture* tex, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordPos = p0;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 500;
    m_currentFrame.h = 500; // HARD code aiyayai

    m_rectRegionPos = YRect(YPos(p0.x(), p0.y()), YPos(p0.x() + 500, p0.y() + 500));
}

void RenderBackground::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRender = m_coordPos - state.getCoordLeftTop();
    m_rectRegionRender = m_rectRegionPos - YPos(state.getCoordLeftTop().x(), state.getCoordLeftTop().y());
};

bool RenderBackground::canDraw(const GameState& state) {
    return m_rectRegionRender.hasIntersection(state.getWindowRect());
    // return true;
}

void RenderBackground::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_rectRegionRender.getMinX();
    dst.y = m_rectRegionRender.getMinY();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};
