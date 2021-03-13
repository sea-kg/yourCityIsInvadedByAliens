#include "render_player_alient_ship.h"
#include "wsjcpp_core.h"
#include "sea5kg_triangulation.h"

// ---------------------------------------------------------------------
// RenderPlayerAlienShip

RenderPlayerAlienShip::RenderPlayerAlienShip(const CoordXY &p0, SDL_Texture* tex, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordCenter = p0;
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
    }
};

void RenderPlayerAlienShip::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordCenter.x();
    dst.y = m_coordCenter.y();
    dst.w = currentFrame.w;
    dst.h = currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};
