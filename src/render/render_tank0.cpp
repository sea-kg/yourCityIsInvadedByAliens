

#include "render_tank0.h"

// ---------------------------------------------------------------------
// RenderTank0

RenderTank0::RenderTank0(GameTank0State *pTank0State, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTank0State = pTank0State;
    m_pAiTank0 = new AiTank0();
    m_pTexture = tex;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 50;
    m_currentFrame.h = 50;
    m_nPrevPosition = 0;
}

void RenderTank0::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 200;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pTank0State->getPosition() - state.getCoordLeftTop();
        return; // skip - already desition done
    }

    m_nPrevPosition = position;

    m_pAiTank0->makeStep(*m_pTank0State);

    m_coordRender = m_pTank0State->getPosition() - state.getCoordLeftTop();

    MoveObjectDirection dr = m_pTank0State->getDirection();

    GameRocketState *pRocketState = m_pTank0State->popRocket();
    if (pRocketState != nullptr) {
        pRenderWindow->addRocket(pRocketState);
    }

    int nWidth = 50;
    if (!m_pTank0State->hasRocket()) {
        switch(dr) {
            case MoveObjectDirection::UP:
                m_currentFrame.x = nWidth*2;
                m_currentFrame.y = nWidth;
                break;
            case MoveObjectDirection::UP_LEFT:
                m_currentFrame.x = nWidth;
                m_currentFrame.y = nWidth;
                break;
            case MoveObjectDirection::UP_RIGHT:
                m_currentFrame.x = nWidth*3;
                m_currentFrame.y = nWidth;
                break;
            case MoveObjectDirection::DOWN:
                m_currentFrame.x = nWidth*2;
                m_currentFrame.y = nWidth*3;
                break;
            case MoveObjectDirection::DOWN_LEFT:
                m_currentFrame.x = nWidth;
                m_currentFrame.y = nWidth*3;
                break;
            case MoveObjectDirection::DOWN_RIGHT:
                m_currentFrame.x = nWidth*3;
                m_currentFrame.y = nWidth*3;
                break;
            case MoveObjectDirection::LEFT:
                m_currentFrame.x = nWidth;
                m_currentFrame.y = nWidth*2;
                break;
            case MoveObjectDirection::RIGHT:
                m_currentFrame.x = nWidth*3;
                m_currentFrame.y = nWidth*2;
                break;
        }
    } else {
        switch(dr) {
            case MoveObjectDirection::UP:
                m_currentFrame.x = nWidth*2;
                m_currentFrame.y = 0;
                break;
            case MoveObjectDirection::UP_LEFT:
                m_currentFrame.x = 0;
                m_currentFrame.y = 0;
                break;
            case MoveObjectDirection::UP_RIGHT:
                m_currentFrame.x = nWidth*4;
                m_currentFrame.y = 0;
                break;
            case MoveObjectDirection::DOWN:
                m_currentFrame.x = nWidth*2;
                m_currentFrame.y = nWidth*4;
                break;
            case MoveObjectDirection::DOWN_LEFT:
                m_currentFrame.x = 0;
                m_currentFrame.y = nWidth*4;
                break;
            case MoveObjectDirection::DOWN_RIGHT:
                m_currentFrame.x = nWidth*4;
                m_currentFrame.y = nWidth*4;
                break;
            case MoveObjectDirection::LEFT:
                m_currentFrame.x = 0;
                m_currentFrame.y = nWidth*2;
                break;
            case MoveObjectDirection::RIGHT:
                m_currentFrame.x = nWidth*4;
                m_currentFrame.y = nWidth*2;
                break;
        }
    }

    
};

void RenderTank0::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.x() - 25;
    dst.y = m_coordRender.y() - 25;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};