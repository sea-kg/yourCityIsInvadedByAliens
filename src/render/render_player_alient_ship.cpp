#include "render_player_alient_ship.h"
#include "wsjcpp_core.h"
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
    // m_coordReal = m_coordCenter + state.getCoordLeftTop();
    
    long m_nSpeedAnimation = 100;
    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        
        // m_coordReal = m_coordCenter + state.getCoordLeftTop();
        return; // skip - already desition done
    }
    m_nPrevPosition = position;

    currentFrame.y = (position % 25) * 100;

    if (state.isPlayerShooting()) {
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
    dst.x =  m_pState->getPosition().x();
    dst.y = m_pState->getPosition().y();
    dst.w = currentFrame.w;
    dst.h = currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};
