#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeProgressBar;

class YAssetFactoryProgressBar : public YAssetFactory {
    public:
        YAssetFactoryProgressBar(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeProgressBar *pFactoryTypeFont,
            const std::wstring &sImagePath,
            int nWidth,
            int nHeight
        );
        virtual std::unique_ptr<YAsset> createAsset() override;

    private:
        YAssetFactoryTypeProgressBar *m_pFactoryTypeFont;
        std::wstring m_sImagePath;
        SDL_Texture *m_pTexture;
        int m_nWidth;
        int m_nHeight;
};

class YAssetFactoryTypeProgressBar : public YAssetFactoryType {
    public:
        YAssetFactoryTypeProgressBar(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() const override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        static const inline std::wstring m_sType{L"progressbar"};
        std::wstring TAG;
};
