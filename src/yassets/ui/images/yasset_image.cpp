#include "yasset_image.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetImage

YAssetImage::YAssetImage(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    int nWidth,
    int nHeight
)
: YAsset(pAssetsService), RenderObject(1000) {
    m_pTexture = pTexture;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nWidth;
    m_currentFrame.h = m_nHeight;
}

void YAssetImage::setAbsolutePosition(bool bAbsolutePosition) {
    m_bAbsolutePosition = bAbsolutePosition;
}

void YAssetImage::setPosition(int nX, int nY) {
    m_nX = nX;
    m_nY = nY;
}

void YAssetImage::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetImage::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (m_needUpdate.isAndDoReset()) {
        // TODO
    }
};

bool YAssetImage::canDraw(const GameState& state) {
    return true;
}

void YAssetImage::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_nX;
    dst.y = m_nY;
    dst.w = m_nWidth;
    dst.h = m_nHeight;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};
