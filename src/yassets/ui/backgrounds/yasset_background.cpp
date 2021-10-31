#include "yasset_background.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetBackground

YAssetBackground::YAssetBackground(
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

    auto *pWindow = findYService<WindowYService>();
    m_nWindowHeight = pWindow->getHeight();
    m_nWindowWidth = pWindow->getWidth();

}

void YAssetBackground::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetBackground::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (m_needUpdate.isAndDoReset()) {
        // TODO
    }
};

bool YAssetBackground::canDraw(const GameState& state) {
    return true;
}

void YAssetBackground::draw(SDL_Renderer* renderer) {
    for (int x = 0; x < m_nWindowWidth*2; x += m_nWidth) {
        for (int y = 0; y < m_nWindowHeight*2; y += m_nHeight) {
            SDL_Rect dst;
            dst.x = x;
            dst.y = y;
            dst.w = m_nWidth;
            dst.h = m_nHeight;
            SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
        }
    }
};
