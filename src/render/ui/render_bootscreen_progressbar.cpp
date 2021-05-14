#include "render_bootscreen_progressbar.h"

// ---------------------------------------------------------------------
// RenderBootScreenProgressBar

RenderBootScreenProgressBar::RenderBootScreenProgressBar(
    SDL_Texture* tex,
    CoordXY pos,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_position = pos;
    m_nCurProgress = 50;
    m_nMaxProgress = 100;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 500;
    m_currentFrame.h = 50;

    m_currentFrame2.x = 0;
    m_currentFrame2.y = 50;
    m_currentFrame2.w = 500;
    m_currentFrame2.h = 50;
}

void RenderBootScreenProgressBar::updateProgress(int nProgressMax, int nProgressCurrent) {
    m_nCurProgress = nProgressCurrent;
    m_nMaxProgress = nProgressMax;
}

void RenderBootScreenProgressBar::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    if (m_nMaxProgress > 0) {
        m_currentFrame2.w = (m_nCurProgress * 500) / m_nMaxProgress;
    } else {
        m_currentFrame2.w = 0;
    }
};

bool RenderBootScreenProgressBar::canDraw(const GameState& state) {
    return true;
}

void RenderBootScreenProgressBar::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = m_position.x();
    dst.y = m_position.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);
    
    dst.x = m_position.x();
    dst.y = m_position.y();
    dst.w = m_currentFrame2.w;
    dst.h = m_currentFrame2.h;
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame2, &dst);

};
