#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeTank;

class YAssetFactoryTank : public YAssetFactory {
    public:
        YAssetFactoryTank(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeTank *pFactoryTypeTank,
            const std::wstring &sSpriteTankPath,
            const std::wstring &sSpriteRocketPath,
            int nFrameWidth,
            int nFrameHeight
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFactoryTypeTank *m_pFactoryTypeTank;
        std::wstring m_sSpriteTankPath;
        std::wstring m_sSpriteRocketPath;
        SDL_Texture * m_pTextureTank;
        SDL_Texture * m_pTextureRocket;
        int m_nFrameWidth;
        int m_nFrameHeight;
};

class YAssetFactoryTypeTank : public YAssetFactoryType {
    public:
        YAssetFactoryTypeTank(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
