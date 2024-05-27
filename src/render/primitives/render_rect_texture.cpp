#include "render_rect_texture.h"

// ---------------------------------------------------------------------
// RenderRectTexture

RenderRectTexture::RenderRectTexture(
    const YPos &p0,
    SDL_Texture* tex,
    int nTextureWidth,
    int nTextureHeight,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_pTexture = tex;
    m_coordPos = p0;
    m_coordPosEnd = YPos(p0.getX() + nTextureWidth, p0.getY() + nTextureHeight);
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = nTextureWidth;
    currentFrame.h = nTextureHeight;
}

void RenderRectTexture::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_coordRender = m_coordPos - state.getCoordLeftTop();
    m_coordRenderEnd =  m_coordPosEnd - state.getCoordLeftTop();
};

bool RenderRectTexture::canDraw(const GameState& state) {
    return
        m_coordRender.isInsideRect(state.getWindowRect())
        || m_coordRenderEnd.isInsideRect(state.getWindowRect())
    ;
}

void RenderRectTexture::draw(SDL_Renderer* renderer) {
    SDL_Rect dst;
    // 4 is scale
    dst.x = m_coordRender.getX();
    dst.y = m_coordRender.getY();
    dst.w = currentFrame.w;
    dst.h = currentFrame.h;

    SDL_RenderCopy(renderer, m_pTexture, &currentFrame, &dst);
};