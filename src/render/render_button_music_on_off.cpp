#include "render_button_music_on_off.h"

// ---------------------------------------------------------------------
// RenderButtonMusicOnOff

RenderButtonMusicOnOff::RenderButtonMusicOnOff(
    SDL_Texture* tex, 
    CoordXY pos,
    int nPositionZ
) : RenderObject(nPositionZ) {
    m_position = pos;
    m_pTexture = tex;
    m_bPlayMusic = false;
    m_bAnimate = false;
    SDL_Point size;
    SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
    m_nTextureWidth = size.x;
    m_nTextureHeight = size.y;

    m_currentFrameMusicOn.x = 0;
    m_currentFrameMusicOn.y = 0;
    m_currentFrameMusicOn.w = m_nTextureWidth;
    m_currentFrameMusicOn.h = m_nTextureHeight/2;

    m_currentFrameMusicOff.x = 0;
    m_currentFrameMusicOff.y = m_nTextureHeight/2;
    m_currentFrameMusicOff.w = m_nTextureWidth;
    m_currentFrameMusicOff.h = m_nTextureHeight/2;
}

void RenderButtonMusicOnOff::modify(const GameState& state, IRenderWindow* pRenderWindow) {
    m_bPlayMusic = state.isPlayMusic();
};

void RenderButtonMusicOnOff::draw(SDL_Renderer* renderer) {
    RenderColor emptyColor(0, 0, 0, 0);
    emptyColor.changeRenderColor(renderer);
    SDL_Rect dst;
    dst.x = m_position.x();
    dst.y = m_position.y();
    float nCoef = 1;
    if (m_bAnimate) {
        nCoef = 1.2;
        dst.x = dst.x - (m_currentFrameMusicOn.w * nCoef - m_currentFrameMusicOn.w)/2;
        dst.y = dst.y - (m_currentFrameMusicOn.h * nCoef - m_currentFrameMusicOn.h)/2;
    }

    if (m_bPlayMusic) {
        dst.w = m_currentFrameMusicOn.w * nCoef;
        dst.h = m_currentFrameMusicOn.h * nCoef;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameMusicOn, &dst);    
    } else {
        dst.w = m_currentFrameMusicOff.w * nCoef;
        dst.h = m_currentFrameMusicOff.h * nCoef;
        SDL_RenderCopy(renderer, m_pTexture, &m_currentFrameMusicOff, &dst);
    }
};

void RenderButtonMusicOnOff::setAnimate(bool bAnimate) {
    m_bAnimate = bAnimate;
}