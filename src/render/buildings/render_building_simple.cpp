#include "render_building_simple.h"


// ---------------------------------------------------------------------
// RenderBuildingSimple

RenderBuildingSimple::RenderBuildingSimple(const YPos &pos, SDL_Texture* pTexture)
: RenderObject(600) {
    m_pTexture = pTexture;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 200;
    m_currentFrame.h = 300;

    m_coordAbsolute = CoordXY(pos.getX(), pos.getY());
    // m_maxPos = pos + YPos(200,300);
}

void RenderBuildingSimple::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRender = m_coordAbsolute - state.getCoordLeftTop();
    // m_coordRenderEnd = m_coordPosEnd - state.getCoordLeftTop();

    /*for (int i = 0; i < m_vBorderLines.size(); i++) {
        m_vBorderLines[i]->modify(state, pRenderWindow);
    }

    for (int i = 0; i < m_vFillPointsAbsolute.size(); i++) {
        m_vFillPoints[i] = m_vFillPointsAbsolute[i] - state.getCoordLeftTop();
    }

    for (int i = 0; i < m_vLines.size(); i++) {
        m_vLines[i]->modify(state, pRenderWindow);
    }*/
}

bool RenderBuildingSimple::canDraw(const GameState& state) {
    /*return
        m_minPos.isInsideRect(state.getWindowRect())
        || m_maxPos.isInsideRect(state.getWindowRect())
    ;*/
    return true;
}

void RenderBuildingSimple::draw(SDL_Renderer* renderer) {

    SDL_Rect dst;
    dst.x = m_coordRender.x();
    dst.y = m_coordRender.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
}