#pragma once

#include <yassets_service.h>

class YAssetImage : public YAsset, public RenderObject {
    public:
        YAssetImage(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            int nWidth,
            int nHeight
        );

        void setAbsolutePosition(bool bAbsolutePosition);
        void setPosition(int nX, int nY);

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        std::wstring TAG;
        bool m_bAbsolutePosition;
        int m_nX;
        int m_nY;
        int m_nWidth;
        int m_nHeight;
        int m_nWindowWidth;
        int m_nWindowHeight;
        SDL_Texture *m_pTexture;
        SDL_Rect m_currentFrame;
};


YASSET_DECLARE_INLINE(YAssetImage)
