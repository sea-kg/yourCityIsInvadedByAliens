

#include "render_rocket.h"
#include <iostream>

// ---------------------------------------------------------------------
// RenderRocket

RenderRocket::RenderRocket(GameRocketState *pRocketState, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pRocketState = pRocketState;
    m_pTexture = tex;
    m_size = YPos(50,50);
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_size.getX();
    m_currentFrame.h = m_size.getY();
    m_nPrevPosition = 0;
    m_nLifeTime = 0;
    m_nMaxLifeTime = 50;
    m_nSpeedAnimation = 50;
}

RenderRocket::~RenderRocket() {
    // delete m_pRocketState;
}

void RenderRocket::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos();
        m_coordRenderEnd = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos() + m_size;
        return; // skip - already desition done
    }

    if (m_nLifeTime < m_nMaxLifeTime && m_pRocketState->isExploded()) {
        m_nLifeTime = m_nMaxLifeTime;
    }

    m_nPrevPosition = position;
    
    if (m_nLifeTime == m_nMaxLifeTime) {
        m_pRocketState->destroy();
        m_nLifeTime++;
        m_currentFrame.x = 50;
        m_currentFrame.y = 50;
        return;
    }

    if (m_nLifeTime > m_nMaxLifeTime) {
        m_nLifeTime++;
        if (m_nLifeTime > m_nMaxLifeTime + 2) {
            // remove object from rendering
            this->destroy();
        }
        return;
    }

    m_pRocketState->move();
    m_nLifeTime++;

    m_coordRender = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos();
    m_coordRenderEnd = m_pRocketState->getPosition() - state.getCoordLeftTop().toYPos() + m_size;
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

bool RenderRocket::canDraw(const GameState& state) {
    return
        state.getWindowRect().hasPos(m_coordRender)
        || state.getWindowRect().hasPos(m_coordRenderEnd)
    ;
}

void RenderRocket::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.getX() - 25;
    dst.y = m_coordRender.getY() - 25;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};