#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeFont;

class YAssetFactoryFont : public YAssetFactory {
    public:
        YAssetFactoryFont(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeFont *pFactoryTypeFont,
            const std::string &sImagePath,
            const std::string &sAlphabet
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFactoryTypeFont *m_pFactoryTypeFont;
        std::string m_sImagePath;
        std::string m_sAlphabet;
        SDL_Texture *m_pTexture;
};

class YAssetFactoryTypeFont : public YAssetFactoryType {
    public:
        YAssetFactoryTypeFont(YAssetsService *pAssetsService);
        virtual std::string getFabricTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::string &sAssetFactoryPath,
            const std::string &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::string TAG;
};
