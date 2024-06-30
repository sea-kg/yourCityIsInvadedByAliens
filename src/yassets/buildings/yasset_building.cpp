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
    m_nTextureHeight(nHeight),
    m_currentFrame({0,0,nWidth,nHeight})
{
    // nothing
}

void YAssetBuilding::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetBuilding::setAbsolutePosition(const YPos &pos) {
    m_coordAbsolutePositionTopLeft = pos;
    m_coordAbsolutePositionTopRight = YPos(pos.getX() + m_currentFrame.w, pos.getY());
    m_coordAbsolutePositionBottomLeft = YPos(pos.getX(), pos.getY() + m_currentFrame.h);
    m_coordAbsolutePositionBottomRight = YPos(pos.getX() + m_currentFrame.w, pos.getY() + m_currentFrame.h);
}

int YAssetBuilding::getWidth() const {
    return m_currentFrame.w;
}

int YAssetBuilding::getHeight() const {
    return m_currentFrame.h;
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

    SDL_Rect dst{
        m_coordRenderTopLeft.getX(),
        m_coordRenderTopLeft.getY(),
        m_currentFrame.w,
        m_currentFrame.h
    };
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
}
