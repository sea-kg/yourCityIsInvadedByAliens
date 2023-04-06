#include "yassets_factory_type_screen_border_flash_highlight.h"
#include "yasset_screen_border_flash_highlight.h"

// ---------------------------------------------------------------------
// YAssetFactoryScreenBorderFlashHighlight

YAssetFactoryScreenBorderFlashHighlight::YAssetFactoryScreenBorderFlashHighlight(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeScreenBorderFlashHighlight *pFactoryType,
    const std::wstring &sImagePath,
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
) : YAssetFactory(pAssetsService, pFactoryType) {
    TAG = L"YAssetFactoryScreenBorderFlashHighlight";
    m_pFactoryType = pFactoryType;
    m_sImagePath = sImagePath;
    m_pTexture = loadTexture(sImagePath);
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nLeftTopAngleWidth = nLeftTopAngleWidth;
    m_nLeftTopAngleHeight = nLeftTopAngleHeight;
    m_nLeftBottomAngleWidth = nLeftBottomAngleWidth;
    m_nLeftBottomAngleHeight = nLeftBottomAngleHeight;
    m_nRightTopAngleWidth = nRightTopAngleWidth;
    m_nRightTopAngleHeight = nRightTopAngleHeight;
    m_nRightBottomAngleWidth = nRightBottomAngleWidth;
    m_nRightBottomAngleHeight = nRightBottomAngleHeight;
    m_nMiddleAreaMinX = nMiddleAreaMinX;
    m_nMiddleAreaMaxX = nMiddleAreaMaxX;
    m_nMiddleAreaMinY = nMiddleAreaMinY;
    m_nMiddleAreaMaxY = nMiddleAreaMaxY;
}

YAsset *YAssetFactoryScreenBorderFlashHighlight::createAsset() {
    return new YAssetScreenBorderFlashHighlight(
        m_pAssetsService,
        m_pTexture,
        m_nWidth,
        m_nHeight,
        m_nLeftTopAngleWidth,
        m_nLeftTopAngleHeight,
        m_nLeftBottomAngleWidth,
        m_nLeftBottomAngleHeight,
        m_nRightTopAngleWidth,
        m_nRightTopAngleHeight,
        m_nRightBottomAngleWidth,
        m_nRightBottomAngleHeight,
        m_nMiddleAreaMinX,
        m_nMiddleAreaMaxX,
        m_nMiddleAreaMinY,
        m_nMiddleAreaMaxY
    );
    return nullptr;
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeScreenBorderFlashHighlight

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeScreenBorderFlashHighlight)

YAssetFactoryTypeScreenBorderFlashHighlight::YAssetFactoryTypeScreenBorderFlashHighlight(YAssetsService *pAssetsService) 
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeScreenBorderFlashHighlight";
}

std::wstring YAssetFactoryTypeScreenBorderFlashHighlight::getFactoryTypeId() {
    return L"screen-border-flash-highlight";
}

YAssetFactory *YAssetFactoryTypeScreenBorderFlashHighlight::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sSpritePath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"image"].getString();
    if (!YCore::fileExists(sSpritePath)) {
        YLog::throw_err(TAG, L"Did not find imege " + sSpritePath);
    }

    // TODO
    int nWidth = jsonFactoryConfig[L"image-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"image-height"].getNumber();
    int nLeftTopAngleWidth = jsonFactoryConfig[L"left-top-angle-width"].getNumber();
    int nLeftTopAngleHeight = jsonFactoryConfig[L"left-top-angle-height"].getNumber();
    int nLeftBottomAngleWidth = jsonFactoryConfig[L"left-bottom-angle-width"].getNumber();
    int nLeftBottomAngleHeight = jsonFactoryConfig[L"left-bottom-angle-height"].getNumber();
    int nRightTopAngleWidth = jsonFactoryConfig[L"right-top-angle-width"].getNumber();
    int nRightTopAngleHeight = jsonFactoryConfig[L"right-top-angle-height"].getNumber();
    int nRightBottomAngleWidth = jsonFactoryConfig[L"right-bottom-angle-width"].getNumber();
    int nRightBottomAngleHeight = jsonFactoryConfig[L"right-bottom-angle-height"].getNumber();
    int nMiddleAreaMinX = jsonFactoryConfig[L"middle-area-min-x"].getNumber();
    int nMiddleAreaMaxX = jsonFactoryConfig[L"middle-area-max-x"].getNumber();
    int nMiddleAreaMinY = jsonFactoryConfig[L"middle-area-min-y"].getNumber();
    int nMiddleAreaMaxY = jsonFactoryConfig[L"middle-area-max-y"].getNumber();

    return new YAssetFactoryScreenBorderFlashHighlight(
        m_pAssetsService, 
        this,
        sSpritePath,
        nWidth,
        nHeight,
        nLeftTopAngleWidth,
        nLeftTopAngleHeight,
        nLeftBottomAngleWidth,
        nLeftBottomAngleHeight,
        nRightTopAngleWidth,
        nRightTopAngleHeight,
        nRightBottomAngleWidth,
        nRightBottomAngleHeight,
        nMiddleAreaMinX,
        nMiddleAreaMaxX,
        nMiddleAreaMinY,
        nMiddleAreaMaxY
    );
}
