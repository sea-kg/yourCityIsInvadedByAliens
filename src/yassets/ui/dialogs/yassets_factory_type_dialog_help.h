#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeDialogHelp;

class YAssetFactoryDialogHelp : public YAssetFactory {
    public:
        YAssetFactoryDialogHelp(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeDialogHelp *pFactoryTypeFont,
            const std::wstring &sImageHelpPath,
            int nHelpWidth,
            int nHelpHeight,
            const std::wstring &sImageDialogBackgroundPath,
            int nDialogBackgroundWidth,
            int nDialogBackgroundHeight
        );
        virtual YAsset *createAsset() override;

    private:
        YAssetFactoryTypeDialogHelp *m_pFactoryTypeFont;
        std::wstring m_sImageHelpPath;
        SDL_Texture *m_pTextureHelp;
        int m_nHelpWidth;
        int m_nHelpHeight;
        std::wstring m_sImageDialogBackgroundPath;
        SDL_Texture *m_pTextureDialogBackground;
        int m_nDialogBackgroundWidth;
        int m_nDialogBackgroundHeight;
};

class YAssetFactoryTypeDialogHelp : public YAssetFactoryType {
    public:
        YAssetFactoryTypeDialogHelp(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
