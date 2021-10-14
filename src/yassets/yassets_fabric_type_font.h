#pragma once

#include <yassets_service.h>

class YAssetFabricTypeFont;

class YAssetFactoryFont : public YAssetFactory {
    public:
        YAssetFactoryFont(YAssetsService *pAssetsService, YAssetFabricTypeFont *pFactoryTypeFont);

    private:
        YAssetFabricTypeFont *m_pFactoryTypeFont;
};

class YAssetFabricTypeFont : public YAssetFactoryType {
    public:
        YAssetFabricTypeFont(YAssetsService *pAssetsService);
        virtual std::string getFabricTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::string &sAssetFactoryPath,
            const std::string &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::string TAG;
};
