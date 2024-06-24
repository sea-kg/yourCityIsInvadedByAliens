#include "yasset_building.h"
#include "render.h"


// ---------------------------------------------------------------------
// YAssetBuilding

YAssetBuilding::YAssetBuilding(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    int nWidth,
    int nHeight
) :
    YAsset(pAssetsService),
    RenderObject(1000),
    m_pTexture(pTexture),
    m_nTextureWidth(nWidth),
    m_nTextureHeight(nHeight)
{
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nTextureWidth;
    m_currentFrame.h = m_nTextureHeight;
}

void YAssetBuilding::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetBuilding::setAbsolutePosition(const YPos &coordPos) {
    m_coordAbsolutePositionTopLeft = coordPos; // + YPos(0, 0);
    m_coordAbsolutePositionTopRight = coordPos + YPos(m_nTextureWidth, 0);
    m_coordAbsolutePositionBottomLeft = coordPos + YPos(0, m_nTextureHeight);
    m_coordAbsolutePositionBottomRight = coordPos + YPos(m_nTextureWidth, m_nTextureHeight);
}

int YAssetBuilding::getWidth() const {
    return m_nTextureWidth;
}

int YAssetBuilding::getHeight() const {
    return m_nTextureHeight;
}

void YAssetBuilding::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRenderTopLeft = m_coordAbsolutePositionTopLeft - state.getCoordLeftTop();
    m_coordRenderTopRight = m_coordAbsolutePositionTopRight - state.getCoordLeftTop();
    m_coordRenderBottomLeft = m_coordAbsolutePositionBottomLeft - state.getCoordLeftTop();
    m_coordRenderBottomRight = m_coordAbsolutePositionBottomRight - state.getCoordLeftTop();
    return;
}

bool YAssetBuilding::canDraw(const GameState& state) {
    return
        m_coordRenderTopLeft.isInsideRect(state.getWindowRect())
        || m_coordRenderTopRight.isInsideRect(state.getWindowRect())
        || m_coordRenderBottomLeft.isInsideRect(state.getWindowRect())
        || m_coordRenderBottomRight.isInsideRect(state.getWindowRect())
    ;
}

void YAssetBuilding::draw(SDL_Renderer* renderer) {

    SDL_Rect dst;
    dst.x = m_coordRenderTopLeft.getX();
    dst.y = m_coordRenderTopLeft.getY();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
}
