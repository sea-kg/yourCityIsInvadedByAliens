#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeClouds;

class YAssetFactoryClouds : public YAssetFactory {
    public:
        YAssetFactoryClouds(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeClouds *pFactoryTypeFont,
            const std::vector<std::wstring> &vImages,
            int nWidth,
            int nHeight
        );
        virtual std::unique_ptr<YAsset> createAsset() override;

    private:
        YAssetFactoryTypeClouds *m_pFactoryTypeFont;
        std::vector<std::wstring> m_vImages;
        std::vector<SDL_Texture *> m_vTextures;
        int m_nWidth;
        int m_nHeight;
};

class YAssetFactoryTypeClouds : public YAssetFactoryType {
    public:
        YAssetFactoryTypeClouds(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() const override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        static const inline std::wstring m_sType{L"clouds"};
        std::wstring TAG;
};
