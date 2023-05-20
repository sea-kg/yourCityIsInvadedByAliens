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
        void setAbsolutePosition(const CoordXY &coordPos);

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coordPos;
        CoordXY m_coordPosEnd;
        CoordXY m_coordRender;
        CoordXY m_coordRenderEnd;
        int m_nTextureTileWidth;
        int m_nTextureTileHeight;

        long m_nPrevPosition;

        SDL_Rect m_currentFrame;
        SDL_Texture* m_pTexture;
};

YASSET_DECLARE_INLINE(YAssetRoad)
