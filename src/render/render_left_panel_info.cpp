

#include "render_left_panel_info.h"

// ---------------------------------------------------------------------
// RenderLeftPanelInfo

RenderLeftPanelInfo::RenderLeftPanelInfo(SDL_Texture* tex,  int nPositionZ) 
: RenderObject(nPositionZ) {
    m_pTexture = tex;
    // m_coordCenter = pTank0State->getPosition();
    int nTopHeight = 105;
    int nBottomHeight = 105;
    int nWidth = 320;
    int nHeight = 500;
    
    m_currentFrameTop.x = 0;
    m_currentFrameTop.y = 0;
    m_currentFrameTop.w = nWidth;
    m_currentFrameTop.h = nTopHeight;
    
    m_currentFrameMiddle.x = 0;
    m_currentFrameMiddle.y = nTopHeight;
    m_currentFrameMiddle.w = nWidth;
    m_currentFrameMiddle.h = nHeight - nTopHeight - nBottomHeight;

    m_currentFrameBottom.x = 0;
    m_currentFrameBottom.y = nHeight - nBottomHeight;
    m_currentFrameBottom.w = nWidth;
    m_currentFrameBottom.h = nBottomHeight;
}

void RenderLeftPanelInfo::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_nWindowWidth = state.getWindowWidth();
    m_nWindowHeight = state.getWindowHeight();
};

void RenderLeftPanelInfo::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);

    SDL_Rect dstTop;
    dstTop.x = m_nWindowWidth - m_currentFrameTop.w;
    dstTop.y = 0;
    dstTop.w = m_currentFrameTop.w;
    dstTop.h = m_currentFrameTop.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameTop, &dstTop);

    // must be for 
    int nStart = m_currentFrameTop.h;
    int nMax = m_nWindowHeight + m_currentFrameMiddle.h;
    int nStep = m_currentFrameMiddle.h;
    for (int nTop = nStart; nTop <= nMax; nTop += nStep) {
        SDL_Rect dstMiddle;
        dstMiddle.x = m_nWindowWidth - m_currentFrameMiddle.w;
        dstMiddle.y = nTop;
        dstMiddle.w = m_currentFrameMiddle.w;
        dstMiddle.h = m_currentFrameMiddle.h;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameMiddle, &dstMiddle);
    }

    SDL_Rect dstBottom;
    dstBottom.x = m_nWindowWidth - m_currentFrameBottom.w;
    dstBottom.y = m_nWindowHeight - m_currentFrameBottom.h;
    dstBottom.w = m_currentFrameBottom.w;
    dstBottom.h = m_currentFrameBottom.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameBottom, &dstBottom);
};