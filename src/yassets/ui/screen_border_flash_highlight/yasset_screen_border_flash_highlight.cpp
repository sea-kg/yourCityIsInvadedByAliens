#include "yasset_screen_border_flash_highlight.h"
#include <render.h>

// ---------------------------------------------------------------------
// YAssetScreenBorderFlashHighlight

YAssetScreenBorderFlashHighlight::YAssetScreenBorderFlashHighlight(
    YAssetsService *pAssetsService,
    SDL_Texture *pTexture,
    int nWidth,
    int nHeight,
    int nLeftTopAngleWidth,
    int nLeftTopAngleHeight,
    int nLeftBottomAngleWidth,
    int nLeftBottomAngleHeight,
    int nRightTopAngleWidth,
    int nRightTopAngleHeight,
    int nRightBottomAngleWidth,
    int nRightBottomAngleHeight,
    int nMiddleAreaMinX,
    int nMiddleAreaMaxX,
    int nMiddleAreaMinY,
    int nMiddleAreaMaxY
)
: YAsset(pAssetsService), RenderObject(1000) {
    m_pTexture = pTexture;
    m_size = YPos(nWidth, nHeight);
    
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_size.getX();
    m_currentFrame.h = m_size.getY();
    // m_nPrevPosition = 0;

    m_frameLeftTop.x = 0;
    m_frameLeftTop.y = 0;
    m_frameLeftTop.w = nLeftTopAngleWidth;
    m_frameLeftTop.h = nLeftTopAngleHeight;

    m_frameLeft.x = 0;
    m_frameLeft.y = nLeftTopAngleHeight;
    m_frameLeft.w = nLeftTopAngleWidth;
    m_frameLeft.h = nHeight - nLeftBottomAngleHeight - nLeftTopAngleHeight;

    m_frameLeftBottom.x = 0;
    m_frameLeftBottom.y = nHeight - nLeftBottomAngleHeight;
    m_frameLeftBottom.w = nLeftBottomAngleWidth;
    m_frameLeftBottom.h = nLeftBottomAngleHeight;

    m_frameTop.x = nLeftTopAngleWidth;
    m_frameTop.y = 0;
    m_frameTop.w = nWidth - nLeftTopAngleWidth - nRightTopAngleWidth;
    m_frameTop.h = nLeftTopAngleHeight;

    m_frameRightTop.x = nWidth - nRightTopAngleWidth;
    m_frameRightTop.y = 0;
    m_frameRightTop.w = nRightTopAngleWidth;
    m_frameRightTop.h = nRightTopAngleHeight;

    m_frameRight.x = nWidth - nRightTopAngleWidth;
    m_frameRight.y = nRightTopAngleHeight;
    m_frameRight.w = nRightTopAngleWidth;
    m_frameRight.h = nHeight - nRightBottomAngleHeight - nRightTopAngleHeight;

    m_frameRightBottom.x = nWidth - nRightTopAngleWidth;
    m_frameRightBottom.y = nHeight - nRightBottomAngleHeight;
    m_frameRightBottom.w = nRightBottomAngleWidth;
    m_frameRightBottom.h = nRightBottomAngleHeight;

    m_frameBottom.x = nRightTopAngleWidth;
    m_frameBottom.y = nHeight - nRightBottomAngleHeight;
    m_frameBottom.w = nWidth - nLeftBottomAngleWidth - nRightBottomAngleWidth;
    m_frameBottom.h = nRightBottomAngleHeight;


    m_frameFlash.x = nMiddleAreaMinX;
    m_frameFlash.y = nMiddleAreaMinY;
    m_frameFlash.w = nMiddleAreaMaxX - nMiddleAreaMinX;
    m_frameFlash.h = nMiddleAreaMaxY - nMiddleAreaMinY;

    m_nLeftTopAngleWidth = nLeftTopAngleWidth;
    m_nLeftTopAngleHeight = nLeftTopAngleHeight;
    m_nLeftBottomAngleWidth = nLeftBottomAngleWidth;
    m_nLeftBottomAngleHeight = nLeftBottomAngleHeight;
    m_nRightTopAngleWidth = nRightTopAngleWidth;
    m_nRightTopAngleHeight = nRightTopAngleHeight;
    m_nRightBottomAngleWidth = nRightBottomAngleWidth;
    m_nRightBottomAngleHeight = nRightBottomAngleHeight;
    m_nMiddleAreaMinX = nMiddleAreaMinX;
    m_nMiddleAreaMaxX = nMiddleAreaMaxX;
    m_nMiddleAreaMinY = nMiddleAreaMinY;
    m_nMiddleAreaMaxY = nMiddleAreaMaxY;

    m_nMoves = 0;
    m_nMiddleFlashesNumber = 0;
    m_nTimeFlashing = 5000;
    m_alpha = 0xff;
    m_bFadingReset = true;
    randomDiff();
}

void YAssetScreenBorderFlashHighlight::setAbsolutePosition(bool bAbsolutePosition) {
    m_bAbsolutePosition = bAbsolutePosition;
}

void YAssetScreenBorderFlashHighlight::setPosition(int nX, int nY) {
    m_position = YPos(nX, nY);
}

void YAssetScreenBorderFlashHighlight::setOrderZ(int nOrder) {
    m_nPositionZ = nOrder;
}

void YAssetScreenBorderFlashHighlight::modify(const GameState& state, IRenderWindow* pRenderWindow) {

    m_nWindowWidth = state.getWindowWidth();
    m_nWindowHeight = state.getWindowHeight();

    if (m_bFadingReset) {
        m_bFadingReset = false;
        m_nTimeStartFlashing = state.getElapsedTime();
        return;
    }

    m_nTimeElapsed = state.getElapsedTime() - m_nTimeStartFlashing;
    if (m_nTimeElapsed < m_nTimeFlashing) {
        m_alpha = 255 - (m_nTimeElapsed * 255 / m_nTimeFlashing);
        // std::wcout << m_alpha << std::endl;
    }

    // long m_nSpeedAnimation = 70; // TODO options
    // long value = state.getElapsedTime() / m_nSpeedAnimation;
// 
    // if (m_nPrevValue == value) {
    //     m_nTimeFading -= 
    //     // m_coordRender = m_position - state.getYPosLeftTop();
    //     return; // skip - already desition done
    // }
// 
    // m_nPrevValue = position;
};

bool YAssetScreenBorderFlashHighlight::canDraw(const GameState& state) {
    return m_alpha != 0;
}

void YAssetScreenBorderFlashHighlight::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    SDL_SetTextureAlphaMod(m_pTexture, m_alpha);
    // SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);

    dst.x = 0;
    dst.y = 0;
    dst.w = m_frameLeftTop.w;
    dst.h = m_frameLeftTop.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameLeftTop, &dst);

    dst.x = 0;
    dst.y = m_frameLeftTop.h;
    dst.w = m_frameLeft.w;
    dst.h = m_nWindowHeight - m_frameLeftTop.h - m_frameLeftBottom.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameLeft, &dst);

    dst.x = 0;
    dst.y = m_nWindowHeight - m_frameLeftBottom.h;
    dst.w = m_frameLeftBottom.w;
    dst.h = m_frameLeftBottom.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameLeftBottom, &dst);

    dst.x = m_frameLeftTop.w;
    dst.y = 0;
    dst.w = m_nWindowWidth - m_frameLeftTop.w - m_frameRightTop.w;
    dst.h = m_frameLeftTop.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameTop, &dst);

    dst.x = m_nWindowWidth - m_frameRightTop.w;
    dst.y = 0;
    dst.w = m_frameRightTop.w;
    dst.h = m_frameRightTop.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameRightTop, &dst);

    dst.x = m_nWindowWidth - m_frameRight.w;
    dst.y = m_frameRightTop.h;
    dst.w = m_frameRight.w;
    dst.h = m_nWindowHeight - m_frameRightTop.h - m_frameRightBottom.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameRight, &dst);

    dst.x = m_nWindowWidth - m_frameRightBottom.w;
    dst.y = m_nWindowHeight - m_frameRightBottom.h;
    dst.w = m_frameRightBottom.w;
    dst.h = m_frameRightBottom.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameRightBottom, &dst);

    dst.x = m_frameLeftBottom.w;
    dst.y = m_nWindowHeight - m_frameRightBottom.h;
    dst.w = m_nWindowWidth - m_frameLeftBottom.w - m_frameRightBottom.w;
    dst.h = m_frameLeftBottom.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_frameBottom, &dst);

    for (int i = 0; i < m_nMiddleFlashesNumber; i++) {
        SDL_RenderCopy(renderer, m_pTexture, &m_frameFlash, &(m_vMiddleFlashesDst[i]));
    }
};

void YAssetScreenBorderFlashHighlight::move() {
    m_nMoves++;
    if (m_nMoves > m_nMaxMoves) {
        randomDiff();
        m_nMoves = 0;
    }
    m_position += m_pDiff;
}

void YAssetScreenBorderFlashHighlight::randomDiff() {
    int nStep = 3;
    int nXpos = std::rand() % (nStep*2 + 1);
    nXpos -= nStep;
    int nYpos = std::rand() % (nStep*2 + 1);
    nYpos -= nStep;
    m_pDiff = YPos(nXpos, nYpos);
    m_nMaxMoves = 20 + std::rand() % 100;
}

int YAssetScreenBorderFlashHighlight::randomFlashX() {
    return (std::rand() % (m_nWindowWidth - m_nLeftTopAngleWidth - m_nRightTopAngleWidth - m_frameFlash.w)) + m_nLeftTopAngleWidth;
}

int YAssetScreenBorderFlashHighlight::randomFlashY() {
    return (std::rand() % (m_nWindowHeight - m_nLeftTopAngleHeight - m_nRightTopAngleHeight - m_frameFlash.h)) + m_nLeftTopAngleHeight;
}

void YAssetScreenBorderFlashHighlight::flash(int time, int middle_flashes) {
    m_nTimeFlashing = time;
    m_bFadingReset = true;
    m_nMiddleFlashesNumber = middle_flashes;
    m_vMiddleFlashesDst.clear();
    for (int i = 0; i < m_nMiddleFlashesNumber; i++) {

        SDL_Rect dst;
        dst.x = randomFlashX();
        dst.y = randomFlashY();
        dst.w = m_frameFlash.w;
        dst.h = m_frameFlash.h;

        m_vMiddleFlashesDst.push_back(dst);
    }
}