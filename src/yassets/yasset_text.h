#pragma once

#include <yassets_service.h>

class YAssetText : public YAsset, public RenderObject {
    public:
        YAssetText(
            YAssetsService *pAssetsService,
            SDL_Texture *pTexture,
            const std::wstring &sAlphabet
        );
        void setAbsolutePosition(bool bAbsolutePosition);
        void setPosition(int nX, int nY);
        void setText(const std::wstring& sText);

        // YAsset
        virtual RenderObject *getRenderObject() override;

        // RenderObject
        virtual void modify(const GameState& state, IRenderWindow* pRenderWindow) override;
        virtual bool canDraw(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        std::wstring TAG;
        bool m_bAbsolutePosition;
        int m_nX;
        int m_nY;
        int m_nFontSize;
        bool m_bUpdatedText;
        std::wstring m_sText;
        std::wstring m_sUpdateText;
        SDL_Texture *m_pTexture;
        std::wstring m_sAlphabet;
};

YASSET_DECLARE_INLINE(YAssetText)

