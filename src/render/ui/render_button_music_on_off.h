
#pragma once

#include "render.h"

class RenderButtonMusicOnOff : public RenderObject {

    public:
        RenderButtonMusicOnOff(
            SDL_Texture* tex,
            YPos pos,
            int nPositionZ = 0
        );
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void setAnimate(bool bAnimate);

    private:
        SDL_Texture* m_pTexture;
        YPos m_position;
        int m_nTextureWidth;
        int m_nTextureHeight;
        bool m_bPlayMusic;
        bool m_bAnimate;

        SDL_Rect m_currentFrameMusicOn;
        SDL_Rect m_currentFrameMusicOff;
};