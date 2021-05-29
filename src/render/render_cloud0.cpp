

#include "render_cloud0.h"

// ---------------------------------------------------------------------
// RenderCloud0

RenderCloud0::RenderCloud0(GameCloud0State *pCloud0State, SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pCloud0State = pCloud0State;
    m_pTexture = tex;
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 500;
    m_currentFrame.h = 500;
    m_nPrevPosition = 0;
}

void RenderCloud0::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 70;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_pCloud0State->getPosition() - state.getCoordLeftTop();
        // m_rectRegionPos = YRect(p0.x(), p0.x() + 300, p0.y(), p0.y() + 300);
        return; // skip - already desition done
    }

    m_nPrevPosition = position;
    m_pCloud0State->move();

    m_coordRender = m_pCloud0State->getPosition() - state.getCoordLeftTop();
    m_coordRenderEnd = CoordXY(m_coordRender.x() + 500, m_coordRender.y() + 500);

    m_rectRegionRender = YRect(
        YPos(
            m_pCloud0State->getPosition().x() - state.getCoordLeftTop().x(),
            m_pCloud0State->getPosition().y() - state.getCoordLeftTop().y()
        ),
        YPos(
            m_pCloud0State->getPosition().x() + 500 - state.getCoordLeftTop().x(),
            m_pCloud0State->getPosition().y() + 500 - state.getCoordLeftTop().y()
        )
    );
};

bool RenderCloud0::canDraw(const GameState& state) {

    return m_rectRegionRender.hasIntersection(state.getWindowRect());

    /*return
        m_coordRender.isInsideRect(state.getWindowRect())
        || m_coordRenderEnd.isInsideRect(state.getWindowRect())
    ;*/
}

void RenderCloud0::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_coordRender.x();
    dst.y = m_coordRender.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};