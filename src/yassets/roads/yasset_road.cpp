#include "yasset_road.h"
#include "render.h"


enum class RoadPart {
    NONE = -1,
    VERTICAL = 0,
    HORIZONTAL = 1,
    RIGHT_DOWN = 2,
    LEFT_DOWN = 3,
    LEFT_UP = 4,
    RIGHT_UP = 5,
    CROSS = 6,
    LEFT_UP_DOWN = 7,
    LEFT_RIGHT_UP = 8,
    RIGHT_UP_DOWN = 9,
    LEFT_RIGHT_DOWN = 10,
};

RoadPart convertStringToRoadPart(const std::wstring &sRoadPart) {
    if (sRoadPart == L"vertical") {
        return RoadPart::VERTICAL;
    } else if (sRoadPart == L"horizontal") {
        return RoadPart::HORIZONTAL;
    } else if (sRoadPart == L"right-down") {
        return RoadPart::RIGHT_DOWN;
    } else if (sRoadPart == L"left-down") {
        return RoadPart::LEFT_DOWN;
    } else if (sRoadPart == L"left-up") {
        return RoadPart::LEFT_UP;
    } else if (sRoadPart == L"right-up") {
        return RoadPart::RIGHT_UP;
    } else if (sRoadPart == L"cross") {
        return RoadPart::CROSS;
    } else if (sRoadPart == L"left-up-down") {
        return RoadPart::LEFT_UP_DOWN;
    } else if (sRoadPart == L"left-right-up") {
        return RoadPart::LEFT_RIGHT_UP;
    } else if (sRoadPart == L"right-up-down") {
        return RoadPart::RIGHT_UP_DOWN;
    } else if (sRoadPart == L"left-right-down") {
        return RoadPart::LEFT_RIGHT_DOWN;
    }
    YLog::throw_err(L"convertStringToRoadPart", L"'" + sRoadPart + L"' not expected");
    return RoadPart::NONE;
};


// ---------------------------------------------------------------------
// YAssetRoad

YAssetRoad::YAssetRoad(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    int nFrameWidth,
    int nFrameHeight
) :
    YAsset(pAssetsService),
    RenderObject(1000),
    m_pTexture(pTexture),
    m_nTextureTileWidth(nFrameWidth),
    m_nTextureTileHeight(nFrameHeight)
{
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nTextureTileWidth;
    m_currentFrame.h = m_nTextureTileHeight;
    m_nPrevPosition = 0;
}

void YAssetRoad::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetRoad::setRoadPart(const std::wstring &sRoadPart) {
    int nTile = (int)convertStringToRoadPart(sRoadPart);
    m_currentFrame.y = m_nTextureTileHeight * nTile;
}

void YAssetRoad::setAbsolutePosition(const YPos &coordPos) {
    m_coordAbsolutePositionTopLeft = coordPos; // + YPos(0, 0);
    m_coordAbsolutePositionTopRight = coordPos + YPos(m_nTextureTileWidth, 0);
    m_coordAbsolutePositionBottomLeft = coordPos + YPos(0, m_nTextureTileHeight);
    m_coordAbsolutePositionBottomRight = coordPos + YPos(m_nTextureTileWidth, m_nTextureTileHeight);
}

int YAssetRoad::getFrameWeight() const {
    return m_nTextureTileWidth;
}

int YAssetRoad::getFrameHeight() const {
    return m_nTextureTileHeight;
}

void YAssetRoad::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRenderTopLeft = m_coordAbsolutePositionTopLeft - state.getCoordLeftTop();
    m_coordRenderTopRight = m_coordAbsolutePositionTopRight - state.getCoordLeftTop();
    m_coordRenderBottomLeft = m_coordAbsolutePositionBottomLeft - state.getCoordLeftTop();
    m_coordRenderBottomRight = m_coordAbsolutePositionBottomRight - state.getCoordLeftTop();
    return;
}

bool YAssetRoad::canDraw(const GameState& state) {
    return
        m_coordRenderTopLeft.isInsideRect(state.getWindowRect())
        || m_coordRenderTopRight.isInsideRect(state.getWindowRect())
        || m_coordRenderBottomLeft.isInsideRect(state.getWindowRect())
        || m_coordRenderBottomRight.isInsideRect(state.getWindowRect())
    ;
}

void YAssetRoad::draw(SDL_Renderer* renderer) {

    SDL_Rect dst;
    dst.x = m_coordRenderTopLeft.getX();
    dst.y = m_coordRenderTopLeft.getY();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
}
