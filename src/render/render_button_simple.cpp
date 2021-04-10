#include "render_button_simple.h"

// ---------------------------------------------------------------------
// RenderButtonSimple

RenderButtonSimple::RenderButtonSimple(
    SDL_Texture* tex, 
    CoordXY pos,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_position = pos;
    m_pTexture = tex;
    m_bAnimate = false;
    SDL_Point size;
    SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
    m_nTextureWidth = size.x;
    m_nTextureHeight = size.y;

    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = m_nTextureWidth;
    m_currentFrame.h = m_nTextureHeight;
}

void RenderButtonSimple::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    // nothing
};

void RenderButtonSimple::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);
    SDL_Rect dst;
    dst.x = m_position.x();
    dst.y = m_position.y();
    dst.w = m_currentFrame.w;
    dst.h = m_currentFrame.h;

    float nCoef = 1;
    if (m_bAnimate) {
        nCoef = 1.2;
        dst.x = dst.x - (m_currentFrame.w * nCoef - m_currentFrame.w)/2;
        dst.y = dst.y - (m_currentFrame.h * nCoef - m_currentFrame.h)/2;
        dst.w = m_currentFrame.w * nCoef;
        dst.h = m_currentFrame.h * nCoef;
    }
    SDL_RenderCopy(renderer, m_pTexture, &m_currentFrame, &dst);    
};

void RenderButtonSimple::setAnimate(bool bAnimate) {
    m_bAnimate = bAnimate;
}