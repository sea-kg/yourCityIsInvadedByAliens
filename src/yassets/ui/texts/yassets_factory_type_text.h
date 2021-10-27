#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeText;

class YAssetFactoryText : public YAssetFactory {
    public:
        YAssetFactoryText(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeText *pFactoryTypeFont,
            const std::wstring &sImagePath,
            const std::vector<std::wstring> &vAlphabets,
            int nLetterHeight,
            int nLetterWidth
        );
        virtual YAsset *createAsset() override;

    private:
        YAssetFactoryTypeText *m_pFactoryTypeFont;
        std::wstring m_sImagePath;
        std::vector<std::wstring> m_vAlphabets;
        SDL_Texture *m_pTexture;
        int m_nLetterHeight;
        int m_nLetterWidth;
};

class YAssetFactoryTypeText : public YAssetFactoryType {
    public:
        YAssetFactoryTypeText(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
