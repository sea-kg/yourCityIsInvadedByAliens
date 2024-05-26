#pragma once

#include "yassets_service.h"


class YAssetRoad : public YAsset, public RenderObject {
    public:
        YAssetRoad(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            int nFrameWidth,
            int nFrameHeight
        );

        void setRoadPart(const std::wstring &sRoadPart);
        void setAbsolutePosition(const YPos &coordPos);

        int getFrameWeight() const;
        int getFrameHeight() const;

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

        int m_nTextureTileWidth;
        int m_nTextureTileHeight;

        long m_nPrevPosition;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};

YASSET_DECLARE_INLINE(YAssetRoad)
