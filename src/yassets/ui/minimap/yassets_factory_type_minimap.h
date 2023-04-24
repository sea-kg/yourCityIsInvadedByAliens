#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeMinimap;

class YAssetFactoryMinimap : public YAssetFactory {
    public:
        YAssetFactoryMinimap(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeMinimap *pFactoryTypeFont,
            const std::wstring &sMinimapPath,
            int nWidth,
            int nHeight
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFactoryTypeMinimap *m_pFactoryTypeFont;
        std::wstring m_sMinimapPath;
        SDL_Texture *m_pTexture;
        int m_nWidth;
        int m_nHeight;
};

class YAssetFactoryTypeMinimap : public YAssetFactoryType {
    public:
        YAssetFactoryTypeMinimap(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
