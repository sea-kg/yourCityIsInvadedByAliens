#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeAlienBerry;

class YAssetFactoryAlienBerry : public YAssetFactory {
    public:
        YAssetFactoryAlienBerry(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeAlienBerry *pFactoryTypeAlienBerry,
            const std::wstring &sSpriteAlienBerryPath,
            int nFrameWidth,
            int nFrameHeight,
            int nFrameNumber
        );
        virtual std::unique_ptr<YAsset> createAsset() override;

    private:
        YAssetFactoryTypeAlienBerry *m_pFactoryTypeAlienBerry;
        std::wstring m_sSpriteAlienBerryPath;
        SDL_Texture * m_pTextureAlienBerry;
        int m_nFrameWidth;
        int m_nFrameHeight;
        int m_nFrameNumber;
};

class YAssetFactoryTypeAlienBerry : public YAssetFactoryType {
    public:
        YAssetFactoryTypeAlienBerry(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() const override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        static const inline std::wstring m_sType{L"alien-berry"};
        std::wstring TAG;
};
