#pragma once

#include <yassets_service.h>

class YAssetProgressBar : public YAsset, public RenderObject {
    public:
        YAssetProgressBar(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            int nWidth,
            int nHeight
        );
        void setAbsolutePosition(bool bAbsolutePosition);
        void setPosition(int nX, int nY);
        void setProgressMax(int nProgressMax);
        void setProgressCurrent(int nProgressCurrent);
        void incrementProgressCurrent();

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
        int m_nProgressCurrent;
        int m_nProgressMax;
        YNeedUpdate m_needUpdate;
        SDL_Texture *m_pTexture;
        SDL_Rect m_currentFrameEmptyProgressBar;
        SDL_Rect m_currentFrameFillProgressBar;
};


YASSET_DECLARE_INLINE(YAssetProgressBar)
