#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeFont50x50;

class YAssetFactoryFont50x50 : public YAssetFactory {
    public:
        YAssetFactoryFont50x50(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeFont50x50 *pFactoryTypeFont,
            const std::wstring &sImagePath,
            const std::vector<std::wstring> &vAlphabets
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFactoryTypeFont50x50 *m_pFactoryTypeFont;
        std::wstring m_sImagePath;
        std::vector<std::wstring> m_vAlphabets;
        SDL_Texture *m_pTexture;
};

class YAssetFactoryTypeFont50x50 : public YAssetFactoryType {
    public:
        YAssetFactoryTypeFont50x50(YAssetsService *pAssetsService);
        virtual std::wstring getFabricTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
