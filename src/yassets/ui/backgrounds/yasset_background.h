#pragma once

#include <yassets_service.h>

class YAssetBackground : public YAsset, public RenderObject {
    public:
        YAssetBackground(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            int nWidth,
            int nHeight
        );

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

        void setAbsolutePosition(const YPos &p0);
        int getWidth() const;
        int getHeight() const;

    private:
        std::wstring TAG;
        int m_nWidth;
        int m_nHeight;
        int m_nWindowWidth;
        int m_nWindowHeight;
        SDL_Texture *m_pTexture;
        SDL_Rect m_currentFrame;

        YPos m_coordPos;
        YRect m_rectRegionPos;
        YPos m_coordRender;
        YRect m_rectRegionRender;
};


YASSET_DECLARE_INLINE(YAssetBackground)
