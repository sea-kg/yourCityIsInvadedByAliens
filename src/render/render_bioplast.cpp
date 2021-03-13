

#include "render_bioplast.h"
#include <iostream>

// ---------------------------------------------------------------------
// RenderBioplast

RenderBioplast::RenderBioplast(GameBioplastState *pState, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pBioplastState = pState;
    m_pTexture = tex;
    m_coordCenter = m_pBioplastState->getPosition();
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 50;
    m_currentFrame.h = 50;
    m_nPrevPosition = 0;
    m_nLifeTime = 0;
    m_nMaxLifeTime = 10;
    m_nSpeedAnimation = 50;
}

RenderBioplast::~RenderBioplast() {
    delete m_pBioplastState;
}

void RenderBioplast::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordReal = m_coordCenter + state.getCoordLeftTop();
        return; // skip - already desition done
    }

    m_nPrevPosition = position;
    
    if (m_nLifeTime == m_nMaxLifeTime) {
        m_pBioplastState->destroy();
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

    m_pBioplastState->move();
    m_nLifeTime++;

    m_coordCenter = m_pBioplastState->getPosition();
    m_coordReal = m_coordCenter + state.getCoordLeftTop();

    // MoveObjectDirection dr = m_pBioplastState->getDirection();

    int nWidth = 50;
    m_currentFrame.y = 50*m_nLifeTime;
};

void RenderBioplast::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordReal.x() - 25;
    dst.y = m_coordReal.y() - 25;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};