#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeImage;

class YAssetFactoryImage : public YAssetFactory {
    public:
        YAssetFactoryImage(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeImage *pFactoryTypeFont,
            const std::wstring &sImagePath,
            int nWidth,
            int nHeight
        );
        virtual YAsset *createAsset() override;

    private:
        YAssetFactoryTypeImage *m_pFactoryTypeFont;
        std::wstring m_sImagePath;
        SDL_Texture *m_pTexture;
        int m_nWidth;
        int m_nHeight;
};

class YAssetFactoryTypeImage : public YAssetFactoryType {
    public:
        YAssetFactoryTypeImage(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
