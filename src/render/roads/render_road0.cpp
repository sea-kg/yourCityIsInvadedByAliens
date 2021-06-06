

#include "render_road0.h"

// ---------------------------------------------------------------------
// RenderRoad0

RenderRoad0::RenderRoad0(const CoordXY &pos, SDL_Texture* tex, RoadPart nTile)
: RenderObject(1000) {
    m_pTexture = tex;
    m_coordPos = pos;
    m_coordPosEnd = CoordXY(pos.x() + 120, pos.y() + 120);
    m_nNumberOfTile = (int)nTile;
    m_currentFrame.x = 0;
    m_currentFrame.y = m_nNumberOfTile * 120;
    m_currentFrame.w = 120;
    m_currentFrame.h = 120;
}

void RenderRoad0::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRender = m_coordPos - state.getCoordLeftTop();
    m_coordRenderEnd = m_coordPosEnd - state.getCoordLeftTop();
};

bool RenderRoad0::canDraw(const GameState& state) {
    return
        m_coordRender.isInsideRect(state.getWindowRect())
        || m_coordRenderEnd.isInsideRect(state.getWindowRect())
    ;
}

void RenderRoad0::draw(SDL_Renderer* renderer) {

    SDL_Rect dst;
    dst.x = m_coordRender.x();
    dst.y = m_coordRender.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};