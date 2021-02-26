

#include "render_rocket.h"

// ---------------------------------------------------------------------
// RenderRocket

RenderRocket::RenderRocket(GameRocketState *pRocketState, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pRocketState = pRocketState;
    m_pTexture = tex;
    m_coordCenter = pRocketState->getPosition();
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 50;
    m_currentFrame.h = 50;
    m_nPrevPosition = 0;
}

void RenderRocket::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 50;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordReal = m_coordCenter + state.getCoordLeftTop();
        return; // skip - already desition done
    }

    m_nPrevPosition = position;
    
    m_pRocketState->move();

    m_coordCenter = m_pRocketState->getPosition();
    m_coordReal = m_coordCenter + state.getCoordLeftTop();

    MoveObjectDirection dr = m_pRocketState->getDirection();


    int nWidth = 50;

    switch(dr) {
        case MoveObjectDirection::UP:
            m_currentFrame.x = nWidth;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_currentFrame.x = nWidth*2;
            m_currentFrame.y = 0;
            break;
        case MoveObjectDirection::DOWN:
            m_currentFrame.x = nWidth;
            m_currentFrame.y = nWidth*2;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = nWidth*2;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_currentFrame.x = nWidth*2;
            m_currentFrame.y = nWidth*2;
            break;
        case MoveObjectDirection::LEFT:
            m_currentFrame.x = 0;
            m_currentFrame.y = nWidth;
            break;
        case MoveObjectDirection::RIGHT:
            m_currentFrame.x = nWidth*2;
            m_currentFrame.y = nWidth;
            break;
    }

};

void RenderRocket::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordReal.x() - 25;
    dst.y = m_coordReal.y() - 25;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};