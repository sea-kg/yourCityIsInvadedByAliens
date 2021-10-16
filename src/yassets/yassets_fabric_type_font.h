#pragma once

#include <yassets_service.h>

class YAssetFabricTypeFont;

class YAssetFactoryFont : public YAssetFactory {
    public:
        YAssetFactoryFont(
            YAssetsService *pAssetsService,
            YAssetFabricTypeFont *pFactoryTypeFont,
            const std::string &sImagePath,
            const std::string &sAlphabet
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFabricTypeFont *m_pFactoryTypeFont;
        std::string m_sImagePath;
        std::string m_sAlphabet;
        SDL_Texture *m_pTexture;
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
