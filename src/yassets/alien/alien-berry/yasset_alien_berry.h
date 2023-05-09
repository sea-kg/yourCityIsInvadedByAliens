#pragma once

#include <yassets_service.h>
#include <game_alien_berry_state.h>

class YAssetAlienBerry : public YAsset, public RenderObject {
    public:
        YAssetAlienBerry(
            YAssetsService *pAssetsService,
            SDL_Texture *pTextureAlienBerry,
            int nFrameWidth,
            int nFrameHeight,
            int nFrameNumber
        );

        // void setAbsolutePosition(bool bAbsolutePosition);
        void setState(GameAlienBerryState *pState);

        int getFrameWidth();
        int getFrameHeight();

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        GameAlienBerryState *m_pState;

        int m_nFrameWidth;
        int m_nFrameHeight;
        int m_nFrameNumber;

        long m_nPrevPosition;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};


YASSET_DECLARE_INLINE(YAssetAlienBerry)
