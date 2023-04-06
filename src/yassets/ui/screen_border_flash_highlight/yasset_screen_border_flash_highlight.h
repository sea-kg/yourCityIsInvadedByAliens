#pragma once

#include <yassets_service.h>

class YAssetScreenBorderFlashHighlight : public YAsset, public RenderObject {
    public:
        YAssetScreenBorderFlashHighlight(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            int nWidth,
            int nHeight,
            int nLeftTopAngleWidth,
            int nLeftTopAngleHeight,
            int nLeftBottomAngleWidth,
            int nLeftBottomAngleHeight,
            int nRightTopAngleWidth,
            int nRightTopAngleHeight,
            int nRightBottomAngleWidth,
            int nRightBottomAngleHeight,
            int nMiddleAreaMinX,
            int nMiddleAreaMaxX,
            int nMiddleAreaMinY,
            int nMiddleAreaMaxY
        );

        void setAbsolutePosition(bool bAbsolutePosition);
        void setPosition(int nX, int nY);

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

        void flash(int time, int middle_flashes);

    private:
        std::wstring TAG;
        bool m_bAbsolutePosition;
        YPos m_position;
        YPos m_size;
        SDL_Texture * m_pTexture;
        SDL_Rect m_currentFrame;
        SDL_Rect m_frameLeftTop;
        SDL_Rect m_frameLeft;
        SDL_Rect m_frameLeftBottom;
        SDL_Rect m_frameTop;
        SDL_Rect m_frameRightTop;
        SDL_Rect m_frameRight;
        SDL_Rect m_frameRightBottom;
        SDL_Rect m_frameBottom;
        SDL_Rect m_frameFlash;
        YRect m_rectRegionRender;

        YPos m_coordRender;
        YPos m_coordRenderEnd;
        long m_nTimeElapsed;
        long m_nTimeStartFlashing;
        long m_nTimeFlashing;
        bool m_bFadingReset;
        int m_nMiddleFlashesNumber;
        std::vector<SDL_Rect> m_vMiddleFlashesDst;
        int8_t m_alpha;

        void move();
        void randomDiff();
        int randomFlashX();
        int randomFlashY();

        
        YPos m_pDiff;
        int m_nMoves;
        int m_nMaxMoves;

        int m_nLeftTopAngleWidth;
        int m_nLeftTopAngleHeight;
        int m_nLeftBottomAngleWidth;
        int m_nLeftBottomAngleHeight;
        int m_nRightTopAngleWidth;
        int m_nRightTopAngleHeight;
        int m_nRightBottomAngleWidth;
        int m_nRightBottomAngleHeight;
        int m_nMiddleAreaMinX;
        int m_nMiddleAreaMaxX;
        int m_nMiddleAreaMinY;
        int m_nMiddleAreaMaxY;

        int m_nWindowWidth;
        int m_nWindowHeight;
};


YASSET_DECLARE_INLINE(YAssetScreenBorderFlashHighlight)
