#include "yasset_progressbar.h"
#include <render.h>
#include <codecvt>

// ---------------------------------------------------------------------
// YAssetProgressBar

YAssetProgressBar::YAssetProgressBar(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    int nWidth,
    int nHeight
)
: YAsset(pAssetsService), RenderObject(1000) {
    m_pTexture = pTexture;
    m_nProgressCurrent = 0;
    m_nProgressMax = 100;
    m_nWidth = nWidth;
    m_nHeight = nHeight;

    m_currentFrameEmptyProgressBar.x = 0;
    m_currentFrameEmptyProgressBar.y = 0;
    m_currentFrameEmptyProgressBar.w = m_nWidth;
    m_currentFrameEmptyProgressBar.h = m_nHeight;

    m_currentFrameFillProgressBar.x = 0;
    m_currentFrameFillProgressBar.y = m_nHeight;
    m_currentFrameFillProgressBar.w = m_nWidth;
    m_currentFrameFillProgressBar.h = m_nHeight;
}

void YAssetProgressBar::setAbsolutePosition(bool bAbsolutePosition) {
    m_bAbsolutePosition = bAbsolutePosition;
}

void YAssetProgressBar::setPosition(int nX, int nY) {
    m_nX = nX;
    m_nY = nY;
}

void YAssetProgressBar::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetProgressBar::setProgressMax(int nProgressMax) {
    m_nProgressMax = nProgressMax;
    if (m_nProgressCurrent > m_nProgressMax) {
        m_nProgressCurrent = m_nProgressMax;
    }
    m_needUpdate.setYes();
}

void YAssetProgressBar::setProgressCurrent(int nProgressCurrent) {
    m_nProgressCurrent = nProgressCurrent;
    if (m_nProgressCurrent > m_nProgressMax) {
        m_nProgressCurrent = m_nProgressMax;
    }
    m_needUpdate.setYes();
}

void YAssetProgressBar::incrementProgressCurrent() {
    m_nProgressCurrent++;
    if (m_nProgressCurrent > m_nProgressMax) {
        m_nProgressCurrent = m_nProgressMax;
    }
    m_needUpdate.setYes();
}

void YAssetProgressBar::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (m_needUpdate.isAndDoReset()) {
        if (m_nProgressMax > 0) {
            m_currentFrameFillProgressBar.w = (m_nProgressCurrent * 500) / m_nProgressMax;
        } else {
            m_currentFrameFillProgressBar.w = 0;
        }
    }
};

bool YAssetProgressBar::canDraw(const GameState& state) {
    return true;
}

void YAssetProgressBar::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_nX;
    dst.y = m_nY;
    dst.w = m_currentFrameEmptyProgressBar.w;
    dst.h = m_currentFrameEmptyProgressBar.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameEmptyProgressBar, &dst);
    
    dst.x = m_nX;
    dst.y = m_nY;
    dst.w = m_currentFrameFillProgressBar.w;
    dst.h = m_currentFrameFillProgressBar.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameFillProgressBar, &dst);
};

