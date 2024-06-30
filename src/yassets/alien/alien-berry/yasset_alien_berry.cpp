#include "yasset_alien_berry.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetAlienBerry

YAssetAlienBerry::YAssetAlienBerry(
    YAssetsService *pAssetsService,
    SDL_Texture *pTextureAlienBerry,
    int nFrameWidth,
    int nFrameHeight,
    int nFrameNumber
)
: YAsset(pAssetsService) {
    // m_pAlienBerry0State = pAlienBerry0State;
    m_pTexture = pTextureAlienBerry;
    m_nFrameWidth = nFrameWidth;
    m_nFrameHeight = nFrameHeight;
    m_nFrameNumber = nFrameNumber;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nFrameWidth;
    m_currentFrame.h = m_nFrameHeight;
    m_nPrevPosition = 0;
}

void YAssetAlienBerry::setState(GameAlienBerryState *pState) {
    m_pState = pState;
}

int YAssetAlienBerry::getFrameWidth() {
    return m_nFrameWidth;
}

int YAssetAlienBerry::getFrameHeight() {
    return m_nFrameHeight;
}

void YAssetAlienBerry::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetAlienBerry::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    const long m_nSpeedAnimation = 200;

    m_coordRender = m_pState->getPosition() - state.getCoordLeftTop();

    long nFrameNumber = (state.getElapsedTime() / m_nSpeedAnimation) % m_nFrameNumber;

    if (m_nPrevPosition == nFrameNumber) {
        return; // skip - already desition done
    }
    m_currentFrame.y = nFrameNumber * m_nFrameHeight;
};

bool YAssetAlienBerry::canDraw(const GameState& state) {
    return
        m_coordRender.isInsideRect(state.getWindowRect())
        || m_coordRenderEnd.isInsideRect(state.getWindowRect())
    ;
}

void YAssetAlienBerry::draw(SDL_Renderer* renderer) {
    // RenderColor emptyColor(0, 0, 0, 0);
    // emptyColor.changeRenderColor(renderer);

    SDL_Rect dst;
    dst.x = m_coordRender.getX(); // - m_nTextureTileWidth/2;
    dst.y = m_coordRender.getY(); // - m_nTextureTileHeight/2;
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};



