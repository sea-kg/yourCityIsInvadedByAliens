#pragma once

#include <yassets_service.h>
#include "map_yservice.h"

class YAssetMinimap : public YAsset, public RenderObject {
    public:
        YAssetMinimap(
            YAssetsService *pAssetsService,
            SDL_Texture *pTextureBackground,
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
        void redrawBackground();
        void redrawPlayerPosition();

        std::wstring TAG;
        bool m_bAbsolutePosition;
        int m_nX;
        int m_nY;
        int m_nWidth;
        int m_nHeight;
        int m_nWindowWidth;
        int m_nWindowHeight;
        SDL_Texture *m_pTextureBackground;
        SDL_Texture *m_pTexturePlayer;
        SDL_Rect m_currentFrame;
        MapYService *m_pMapService;
        
        Uint32* m_pBackgroundMinimapPixels;
        Uint32* m_pDynamicObjectsPixels;
        int m_nBufferPixelsSize;
        int m_nPrevPlayerPositionX;
        int m_nPrevPlayerPositionY;

        float m_nWidthK;
        float m_nHeightK;
        int m_nShiftToLeftByX;

};


YASSET_DECLARE_INLINE(YAssetMinimap)
