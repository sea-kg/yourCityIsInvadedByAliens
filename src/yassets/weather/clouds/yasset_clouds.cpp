#include "yasset_clouds.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetClouds

YAssetClouds::YAssetClouds(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    int nWidth,
    int nHeight
)
: YAsset(pAssetsService), RenderObject(1000) {
    m_pTexture = pTexture;
    m_size = YPos(nWidth, nHeight);
    
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_size.getX();
    m_currentFrame.h = m_size.getY();
    m_nPrevPosition = 0;

    m_nMoves = 0;
    randomDiff();
}

void YAssetClouds::setAbsolutePosition(bool bAbsolutePosition) {
    m_bAbsolutePosition = bAbsolutePosition;
}

void YAssetClouds::setPosition(int nX, int nY) {
    m_position = YPos(nX, nY);
}

void YAssetClouds::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetClouds::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    long m_nSpeedAnimation = 70;

    long position = state.getElapsedTime() / m_nSpeedAnimation;

    if (m_nPrevPosition == position) {
        m_coordRender = m_position - state.getYPosLeftTop();
        // m_rectRegionPos = YRect(p0.x(), p0.x() + 300, p0.y(), p0.y() + 300);
        return; // skip - already desition done
    }

    m_nPrevPosition = position;
    this->move();

    m_coordRender = m_position - state.getYPosLeftTop();
    m_coordRenderEnd = m_coordRender + m_size;

    m_rectRegionRender = YRect(
        m_position - state.getYPosLeftTop(),
        m_position + m_size - state.getYPosLeftTop()
    );
};

bool YAssetClouds::canDraw(const GameState& state) {
    return m_rectRegionRender.hasIntersection(state.getWindowRect());
}

void YAssetClouds::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_coordRender.getX();
    dst.y = m_coordRender.getY();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
};

void YAssetClouds::move() {
    m_nMoves++;
    if (m_nMoves > m_nMaxMoves) {
        randomDiff();
        m_nMoves = 0;
    }
    m_position += m_pDiff;
}

void YAssetClouds::randomDiff() {
    int nStep = 3;
    int nXpos = std::rand() % (nStep*2 + 1);
    nXpos -= nStep;
    int nYpos = std::rand() % (nStep*2 + 1);
    nYpos -= nStep;
    m_pDiff = YPos(nXpos, nYpos);
    m_nMaxMoves = 20 + std::rand() % 100;
}