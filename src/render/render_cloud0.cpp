

#include "render_cloud0.h"

// ---------------------------------------------------------------------
// RenderCloud0

RenderCloud0::RenderCloud0(GameCloud0State *pCloud0State, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pCloud0State = pCloud0State;
    m_pTexture = tex;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 300;
    m_currentFrame.h = 300;
    m_nPrevPosition = 0;
}

void RenderCloud0::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 70;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pCloud0State->getPosition() - state.getCoordLeftTop();
        return; // skip - already desition done
    }

    m_nPrevPosition = position;
    m_pCloud0State->move();

    m_coordRender = m_pCloud0State->getPosition() - state.getCoordLeftTop();
};

void RenderCloud0::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.x();
    dst.y = m_coordRender.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};