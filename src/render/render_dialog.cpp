#include "render_dialog.h"

// ---------------------------------------------------------------------
// RenderDialog

RenderDialog::RenderDialog(
    SDL_Texture* tex, 
    int nDialogWidth,
    int nDialogHeight,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_pTexture = tex;
    // m_coordCenter = pTank0State->getPosition();
    int nTopHeight = 105;
    int nBottomHeight = 105;
    
    SDL_Point size;
    SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);

    int nTexWidth = size.x;
    int nTexHeight = size.y;
    int nTileSizeW = nTexWidth/3;
    int nTileSizeH = nTexHeight/3;
    m_nDialogWidth = nDialogWidth;
    m_nDialogHeight = nDialogHeight;

    m_currentFrameTopLeft.x = 0;
    m_currentFrameTopLeft.y = 0;
    m_currentFrameTopLeft.w = nTileSizeW;
    m_currentFrameTopLeft.h = nTileSizeH;

    m_currentFrameTopMiddle.x = nTileSizeW;
    m_currentFrameTopMiddle.y = 0;
    m_currentFrameTopMiddle.w = nTileSizeW;
    m_currentFrameTopMiddle.h = nTileSizeH;

    m_currentFrameTopRight.x = nTexWidth - nTileSizeW;
    m_currentFrameTopRight.y = 0;
    m_currentFrameTopRight.w = nTileSizeW;
    m_currentFrameTopRight.h = nTileSizeH;

    m_currentFrameLeftMiddle.x = 0;
    m_currentFrameLeftMiddle.y = nTileSizeH;
    m_currentFrameLeftMiddle.w = nTileSizeW;
    m_currentFrameLeftMiddle.h = nTileSizeH;

    m_currentFrameCenter.x = nTileSizeW;
    m_currentFrameCenter.y = nTileSizeH;
    m_currentFrameCenter.w = nTileSizeW;
    m_currentFrameCenter.h = nTileSizeH;

    m_currentFrameRightMiddle.x = nTexHeight - nTileSizeW;
    m_currentFrameRightMiddle.y = nTileSizeH;
    m_currentFrameRightMiddle.w = nTileSizeW;
    m_currentFrameRightMiddle.h = nTileSizeH;

    m_currentFrameBottomLeft.x = 0;
    m_currentFrameBottomLeft.y = nTexHeight - nTileSizeH;
    m_currentFrameBottomLeft.w = nTileSizeW;
    m_currentFrameBottomLeft.h = nTileSizeH;

    m_currentFrameBottomMiddle.x = nTileSizeW;
    m_currentFrameBottomMiddle.y = nTexHeight - nTileSizeH;
    m_currentFrameBottomMiddle.w = nTileSizeW;
    m_currentFrameBottomMiddle.h = nTileSizeH;

    m_currentFrameBottomRight.x = nTexWidth - nTileSizeW;
    m_currentFrameBottomRight.y = nTexHeight - nTileSizeH;
    m_currentFrameBottomRight.w = nTileSizeW;
    m_currentFrameBottomRight.h = nTileSizeH;
}

void RenderDialog::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_nWindowWidth = state.getWindowWidth();
    m_nWindowHeight = state.getWindowHeight();
};

void RenderDialog::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);
    int nStart = 0;
    int nStep = 0;
    int nMax = 0;

    int nWindowWidthCenter = m_nWindowWidth/2;
    int nWindowHeightCenter = m_nWindowHeight/2;

    // top left 
    SDL_Rect dstTopLeft;
    dstTopLeft.x = nWindowWidthCenter - m_nDialogWidth/2;
    dstTopLeft.y = nWindowHeightCenter - m_nDialogHeight/2;
    dstTopLeft.w = m_currentFrameTopLeft.w;
    dstTopLeft.h = m_currentFrameTopLeft.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameTopLeft, &dstTopLeft);

    // top middle
    nStart = dstTopLeft.x + m_currentFrameTopLeft.w;
    nStep = m_currentFrameTopMiddle.w;
    nMax = dstTopLeft.x + m_nDialogWidth - m_currentFrameTopLeft.w;
    for (int nPosX = nStart; nPosX <= nMax; nPosX += nStep) {
        SDL_Rect dstMiddle;
        dstMiddle.x = nPosX;
        dstMiddle.y = dstTopLeft.y;
        dstMiddle.w = m_currentFrameTopMiddle.w;
        dstMiddle.h = m_currentFrameTopMiddle.h;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameTopMiddle, &dstMiddle);
    }

    // top right
    SDL_Rect dstTopRight;
    dstTopRight.x = nWindowWidthCenter + m_nDialogWidth/2 - m_currentFrameTopRight.w;
    dstTopRight.y = nWindowHeightCenter - m_nDialogHeight/2;
    dstTopRight.w = m_currentFrameTopRight.w;
    dstTopRight.h = m_currentFrameTopRight.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameTopRight, &dstTopRight);

    // left middle
    nStart = dstTopLeft.y + m_currentFrameTopLeft.h;
    nStep = m_currentFrameLeftMiddle.h;
    nMax = dstTopLeft.y + m_nDialogHeight - m_currentFrameBottomLeft.h;
    for (int nPosY = nStart; nPosY <= nMax; nPosY += nStep) {
        SDL_Rect dstMiddle;
        dstMiddle.x = dstTopLeft.x;
        dstMiddle.y = nPosY;
        dstMiddle.w = m_currentFrameLeftMiddle.w;
        dstMiddle.h = m_currentFrameLeftMiddle.h;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameLeftMiddle, &dstMiddle);
    }

    // center
    int nStartX = dstTopLeft.x + m_currentFrameTopLeft.w;
    int nStepX = m_currentFrameCenter.w;
    int nMaxX = dstTopLeft.x + m_nDialogWidth - m_currentFrameTopLeft.w;
    int nStartY = dstTopRight.y + m_currentFrameTopRight.h;
    int nStepY = m_currentFrameCenter.h;
    int nMaxY = dstTopRight.y + m_nDialogHeight - m_currentFrameBottomRight.h;
    for (int nPosX = nStartX; nPosX <= nMaxX; nPosX += nStepX) {
        for (int nPosY = nStartY; nPosY <= nMaxY; nPosY += nStepY) {
            SDL_Rect dstMiddle;
            dstMiddle.x = nPosX;
            dstMiddle.y = nPosY;
            dstMiddle.w = m_currentFrameCenter.w;
            dstMiddle.h = m_currentFrameCenter.h;
            SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameCenter, &dstMiddle);
        }
    }

    // right middle
    nStart = dstTopRight.y + m_currentFrameTopRight.h;
    nStep = m_currentFrameRightMiddle.h;
    nMax = dstTopRight.y + m_nDialogHeight - m_currentFrameBottomRight.h;
    for (int nPosY = nStart; nPosY <= nMax; nPosY += nStep) {
        SDL_Rect dstMiddle;
        dstMiddle.x = dstTopRight.x;
        dstMiddle.y = nPosY;
        dstMiddle.w = m_currentFrameRightMiddle.w;
        dstMiddle.h = m_currentFrameRightMiddle.h;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameRightMiddle, &dstMiddle);
    }

    // bottom left 
    SDL_Rect dstBottomLeft;
    dstBottomLeft.x = nWindowWidthCenter - m_nDialogWidth/2;
    dstBottomLeft.y = nWindowHeightCenter + m_nDialogHeight/2 - m_currentFrameBottomLeft.h;
    dstBottomLeft.w = m_currentFrameBottomLeft.w;
    dstBottomLeft.h = m_currentFrameBottomLeft.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameBottomLeft, &dstBottomLeft);

    // bottom middle for
    nStart = dstBottomLeft.x + m_currentFrameTopLeft.w;
    nStep = m_currentFrameBottomMiddle.w;
    nMax = dstBottomLeft.x + m_nDialogWidth - m_currentFrameBottomLeft.w;
    for (int nPosX = nStart; nPosX <= nMax; nPosX += nStep) {
        SDL_Rect dstMiddle;
        dstMiddle.x = nPosX;
        dstMiddle.y = dstBottomLeft.y;
        dstMiddle.w = m_currentFrameBottomMiddle.w;
        dstMiddle.h = m_currentFrameBottomMiddle.h;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameBottomMiddle, &dstMiddle);
    }

    // bottom right
    SDL_Rect dstBottomRight;
    dstBottomRight.x = nWindowWidthCenter + m_nDialogWidth/2 - m_currentFrameBottomRight.w;
    dstBottomRight.y = nWindowHeightCenter + m_nDialogHeight/2 - m_currentFrameBottomRight.h;
    dstBottomRight.w = m_currentFrameBottomRight.w;
    dstBottomRight.h = m_currentFrameBottomRight.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameBottomRight, &dstBottomRight);
};