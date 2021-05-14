#include "render_player_power.h"

// ---------------------------------------------------------------------
// RenderPlayerPower

RenderPlayerPower::RenderPlayerPower(
    SDL_Texture* tex,
    GameAlienShipState *pAlienShipState,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_position = CoordXY(0,0);
    m_pTexture = tex;
    m_pAlienShipState = pAlienShipState;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 280;
    m_currentFrame.h = 50;
}

void RenderPlayerPower::updatePosition(CoordXY pos) {
    m_position = pos;
}

void RenderPlayerPower::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // nothing
    int nCur = m_pAlienShipState->getHelthPoints();
    int nMax = m_pAlienShipState->getMaxHelthPoints();
    int nPos = (nCur*17 - 1) / nMax; // 17 becouse like a round + 1

    if (nPos < -1) {
        nPos = 0;
    }
    m_currentFrame.y = nPos*50;

    // m_pAlienShipState
};

bool RenderPlayerPower::canDraw(const GameState& state) {
    return true;
}

void RenderPlayerPower::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);
    SDL_Rect dst;
    dst.x = m_position.x();
    dst.y = m_position.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);    
};
