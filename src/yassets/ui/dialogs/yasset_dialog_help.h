#pragma once

#include <yassets_service.h>

class YAssetDialogHelp : public YAsset, public RenderObject {
    public:
        YAssetDialogHelp(
            YAssetsService *pAssetsService,
            SDL_Texture *pTextureHelp,
            int nHelpWidth,
            int nHelpHeight,
            SDL_Texture *pTextureDialogBackground,
            int nDialogBackgroundWidth,
            int nDialogBackgroundHeight
        );
        void setShow(bool bShow);

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        std::wstring TAG;
        bool m_bAbsolutePosition;
        bool m_bShow;
        int m_nX;
        int m_nY;
        int m_nHelpWidth;
        int m_nHelpHeight;
        int m_nWindowWidth;
        int m_nWindowHeight;
        SDL_Texture *m_pTextureHelp;
        SDL_Rect m_currentFrame;
        int m_nDialogBackgroundWidth;
        int m_nDialogBackgroundHeight;
        SDL_Texture *m_pTextureDialogBackground;
};


YASSET_DECLARE_INLINE(YAssetDialogHelp)
