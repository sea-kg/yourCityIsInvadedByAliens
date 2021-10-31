#pragma once

#include <yassets_service.h>

class YAssetClouds : public YAsset, public RenderObject {
    public:
        YAssetClouds(
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
        YPos m_position;
        YPos m_size;
        SDL_Texture * m_pTexture;
        SDL_Rect m_currentFrame;
        YRect m_rectRegionRender;

        YPos m_coordRender;
        YPos m_coordRenderEnd;
        long m_nPrevPosition;

        void move();
        void randomDiff();

        
        YPos m_pDiff;
        int m_nMoves;
        int m_nMaxMoves;
};


YASSET_DECLARE_INLINE(YAssetClouds)
