#pragma once

#include "yassets_service.h"


class YAssetBuilding : public YAsset, public RenderObject {
    public:
        YAssetBuilding(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            int nWidth,
            int nHeight
        );

        void setAbsolutePosition(const YPos &coordPos);

        int getWidth() const;
        int getHeight() const;

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        YPos m_coordAbsolutePositionTopLeft;
        YPos m_coordAbsolutePositionTopRight;
        YPos m_coordAbsolutePositionBottomLeft;
        YPos m_coordAbsolutePositionBottomRight;

        YPos m_coordRenderTopLeft;
        YPos m_coordRenderTopRight;
        YPos m_coordRenderBottomLeft;
        YPos m_coordRenderBottomRight;

        int m_nTextureWidth;
        int m_nTextureHeight;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};

YASSET_DECLARE_INLINE(YAssetBuilding)
