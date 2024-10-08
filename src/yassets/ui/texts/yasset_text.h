#pragma once

#include <yassets_service.h>

enum class YAssetTextAnchor {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

class YAssetText : public YAsset, public RenderObject {
    public:
        YAssetText(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            const std::vector<std::wstring> &vAlphabets,
            int nLetterHeight,
            int nLetterWidth
        );
        void setAbsolutePosition(bool bAbsolutePosition);
        void setPosition(int nX, int nY);
        void setAnchor(YAssetTextAnchor nAnchor);
        void setText(const std::wstring& sText);

        void hideText();
        void showText();

        // YAsset
        virtual void setOrderZ(int nOrder) override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        std::wstring TAG;
        bool m_bAbsolutePosition;
        YAssetTextAnchor m_nAnchor;
        int m_nOriginalX;
        int m_nOriginalY;
        int m_nX;
        int m_nY;
        int m_nFontSize;
        bool m_bShowText;
        std::wstring m_sText;
        std::wstring m_sUpdateText;
        SDL_Texture *m_pTexture;
        std::vector<std::wstring> m_vAlphabets;
        int m_nLetterHeight;
        int m_nLetterWidth;

        // WindowSize
        int m_nWindowWidth;
        int m_nWindowHeight;

        void findPosition(SDL_Rect &frame, wchar_t c);
};

YASSET_DECLARE_INLINE(YAssetText)
