#include "yassets_factory_type_minimap.h"
#include "yasset_minimap.h"

// ---------------------------------------------------------------------
// YAssetFactoryMinimap

YAssetFactoryMinimap::YAssetFactoryMinimap(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeMinimap *pFactoryTypeFont,
    const std::wstring &sMinimapPath,
    int nWidth,
    int nHeight
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryMinimap";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sMinimapPath = sMinimapPath;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_pTexture = loadTexture(m_sMinimapPath);
}

std::unique_ptr<YAsset> YAssetFactoryMinimap::createAsset() {
    return std::make_unique<YAssetMinimap>(
        m_pAssetsService,
        m_pTexture,
        m_nWidth,
        m_nHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeMinimap

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeMinimap)

YAssetFactoryTypeMinimap::YAssetFactoryTypeMinimap(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeMinimap";
}

const std::wstring &YAssetFactoryTypeMinimap::getFactoryTypeId() const {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeMinimap::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sMinimapPath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"background-minimap"].getString();
    if (!YCore::fileExists(sMinimapPath)) {
        YLog::throw_err(TAG, L"Did not image " + sMinimapPath);
    }
    int nWidth = jsonFactoryConfig[L"image-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"image-height"].getNumber();

    return new YAssetFactoryMinimap(
        m_pAssetsService,
        this,
        sMinimapPath,
        nWidth,
        nHeight
    );
}
