#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeBackground;

class YAssetFactoryBackground : public YAssetFactory {
    public:
        YAssetFactoryBackground(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeBackground *pFactoryTypeFont,
            const std::wstring &sImagePath,
            int nWidth,
            int nHeight
        );
        virtual YAsset *createAsset() override;

    private:
        YAssetFactoryTypeBackground *m_pFactoryTypeFont;
        std::wstring m_sImagePath;
        SDL_Texture *m_pTexture;
        int m_nWidth;
        int m_nHeight;
};

class YAssetFactoryTypeBackground : public YAssetFactoryType {
    public:
        YAssetFactoryTypeBackground(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        static const inline std::wstring m_sType{L"background"};
        std::wstring TAG;
};
