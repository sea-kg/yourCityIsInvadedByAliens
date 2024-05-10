#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeRoad;

class YAssetFactoryRoad : public YAssetFactory {
    public:
        YAssetFactoryRoad(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeRoad *pFactoryTypeRoad,
            const std::wstring &sRoadImageTilesPath,
            int nFrameWidth,
            int nFrameHeight
        );
        virtual std::unique_ptr<YAsset> createAsset() override;

    private:
        YAssetFactoryTypeRoad *m_pFactoryTypeRoad;
        std::wstring m_sRoadImageTilesPath;
        SDL_Texture * m_pTextureRoad;
        int m_nFrameWidth;
        int m_nFrameHeight;
};

class YAssetFactoryTypeRoad : public YAssetFactoryType {
    public:
        YAssetFactoryTypeRoad(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        static const inline std::wstring m_sType{L"road"};
        std::wstring TAG;
};
