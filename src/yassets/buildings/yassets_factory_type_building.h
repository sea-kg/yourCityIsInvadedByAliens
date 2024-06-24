#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeBuilding;

class YAssetFactoryBuilding : public YAssetFactory {
    public:
        YAssetFactoryBuilding(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeBuilding *pFactoryTypeBuilding,
            const std::wstring &sBuildingImagePath,
            int nWidth,
            int nHeight
        );
        virtual std::unique_ptr<YAsset> createAsset() override;

    private:
        YAssetFactoryTypeBuilding *m_pFactoryTypeBuilding;
        std::wstring m_sBuildingImagePath;
        SDL_Texture * m_pTextureBuilding;
        int m_nWidth;
        int m_nHeight;
};

class YAssetFactoryTypeBuilding : public YAssetFactoryType {
    public:
        YAssetFactoryTypeBuilding(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        static const inline std::wstring m_sType{L"building"};
        std::wstring TAG;
};
