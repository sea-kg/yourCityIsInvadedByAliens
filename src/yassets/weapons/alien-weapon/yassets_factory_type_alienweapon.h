#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeAlienWeapon;

class YAssetFactoryAlienWeapon : public YAssetFactory {
    public:
        YAssetFactoryAlienWeapon(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeAlienWeapon *pFactoryTypeFont,
            const std::wstring &sSpritePath,
            int nFrameWidth,
            int nFrameHeight,
            int nFrameNumber
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFactoryTypeAlienWeapon *m_pFactoryTypeFont;
        std::vector<std::wstring> m_vImages;
        SDL_Texture *m_pTexture;
        std::wstring m_sSpritePath;
        int m_nFrameWidth;
        int m_nFrameHeight;
        int m_nFrameNumber;
};

class YAssetFactoryTypeAlienWeapon : public YAssetFactoryType {
    public:
        YAssetFactoryTypeAlienWeapon(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
