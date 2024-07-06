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
: YAsset(pAssetsService) {
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
    setAbsolutePosition(YPos(0,0));
}

void YAssetBackground::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetBackground::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (m_needUpdate.isAndDoReset()) {
        // TODO
    }
    m_coordRender = m_coordPos - state.getCoordLeftTop();
    m_rectRegionRender = m_rectRegionPos - state.getCoordLeftTop();
};

bool YAssetBackground::canDraw(const GameState& state) {
    return m_rectRegionRender.hasIntersection(state.getWindowRect());
}

void YAssetBackground::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_rectRegionRender.getMinX();
    dst.y = m_rectRegionRender.getMinY();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};

void YAssetBackground::setAbsolutePosition(const YPos &p0) {
    m_coordPos = p0;
    m_rectRegionPos = YRect(YPos(p0.getX(), p0.getY()), YPos(p0.getX() + 500, p0.getY() + 500));
}

int YAssetBackground::getWidth() const {
    return m_nWidth;
}

int YAssetBackground::getHeight() const {
    return m_nHeight;
}