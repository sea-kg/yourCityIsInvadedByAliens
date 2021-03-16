#include "render_player_alient_ship.h"
#include "sea5kg_triangulation.h"

// ---------------------------------------------------------------------
// RenderPlayerAlienShip

RenderPlayerAlienShip::RenderPlayerAlienShip(
    GameAlienShipState *pState,
    SDL_Texture* tex,
    int nPositionZ
) 
: RenderObject(nPositionZ) {
    m_pState = pState;
    m_pTexture = tex;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 100;
    currentFrame.h = 100;
    m_nPrevPosition = 0;
    m_nSpeedAnimation = 100;
}

void RenderPlayerAlienShip::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 100;
    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordPositionRendering = m_pState->getPosition() - state.getCoordLeftTop();
        return; // skip - already desition done
    }
    m_nPrevPosition = position;

    currentFrame.y = (position % 25) * 100;

    if (m_pState->isShooting()) {
        std::cout << "Shooting!!!!";
        m_pState->shot();
    }

    GameBioplastState *pBioplastState = m_pState->popRocket();
    while (pBioplastState != nullptr) {
        pRenderWindow->addBioplast(pBioplastState);
        pBioplastState = m_pState->popRocket();
    }
};

void RenderPlayerAlienShip::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x =  m_coordPositionRendering.x() - 50;
    dst.y = m_coordPositionRendering.y() - 50;
    dst.w = currentFrame.w;
    dst.h = currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};
