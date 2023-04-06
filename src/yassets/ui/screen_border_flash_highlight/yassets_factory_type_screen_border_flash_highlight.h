#pragma once

#include <yassets_service.h>

class YAssetFactoryTypeScreenBorderFlashHighlight;

class YAssetFactoryScreenBorderFlashHighlight : public YAssetFactory {
    public:
        YAssetFactoryScreenBorderFlashHighlight(
            YAssetsService *pAssetsService,
            YAssetFactoryTypeScreenBorderFlashHighlight *pFactoryTypeFont,
            const std::wstring &vImages,
            int nWidth,
            int nHeight,
            int nLeftTopAngleWidth,
            int nLeftTopAngleHeight,
            int nLeftBottomAngleWidth,
            int nLeftBottomAngleHeight,
            int nRightTopAngleWidth,
            int nRightTopAngleHeight,
            int nRightBottomAngleWidth,
            int nRightBottomAngleHeight,
            int nMiddleAreaMinX,
            int nMiddleAreaMaxX,
            int nMiddleAreaMinY,
            int nMiddleAreaMaxY
        );
        virtual YAsset *createAsset() override;
        
    private:
        YAssetFactoryTypeScreenBorderFlashHighlight *m_pFactoryType;
        std::wstring m_sImagePath;
        SDL_Texture *m_pTexture;
        int m_nWidth;
        int m_nHeight;
        int m_nLeftTopAngleWidth;
        int m_nLeftTopAngleHeight;
        int m_nLeftBottomAngleWidth;
        int m_nLeftBottomAngleHeight;
        int m_nRightTopAngleWidth;
        int m_nRightTopAngleHeight;
        int m_nRightBottomAngleWidth;
        int m_nRightBottomAngleHeight;
        int m_nMiddleAreaMinX;
        int m_nMiddleAreaMaxX;
        int m_nMiddleAreaMinY;
        int m_nMiddleAreaMaxY;
};

class YAssetFactoryTypeScreenBorderFlashHighlight : public YAssetFactoryType {
    public:
        YAssetFactoryTypeScreenBorderFlashHighlight(YAssetsService *pAssetsService);
        virtual std::wstring getFactoryTypeId() override;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) override;
    private:
        std::wstring TAG;
};
