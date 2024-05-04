#include "yassets_factory_type_background.h"
#include "yasset_background.h"

// ---------------------------------------------------------------------
// YAssetFactoryBackground

YAssetFactoryBackground::YAssetFactoryBackground(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeBackground *pFactoryTypeFont,
    const std::wstring &sImagePath,
    int nWidth,
    int nHeight
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryBackground";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sImagePath = sImagePath;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    // TODO loadTexture: move to parent class 
    m_pTexture = loadTexture(m_sImagePath);
}

YAsset *YAssetFactoryBackground::createAsset() {
    return new YAssetBackground(
        m_pAssetsService,
        m_pTexture,
        m_nWidth,
        m_nHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeBackground

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeBackground)

YAssetFactoryTypeBackground::YAssetFactoryTypeBackground(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeBackground";
}

const std::wstring &YAssetFactoryTypeBackground::getFactoryTypeId() {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeBackground::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sImagePath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"image"].getString();
    if (!YCore::fileExists(sImagePath)) {
        YLog::throw_err(TAG, L"Did not image " + sImagePath);
    }
    int nWidth = jsonFactoryConfig[L"image-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"image-height"].getNumber();

    return new YAssetFactoryBackground(
        m_pAssetsService,
        this,
        sImagePath,
        nWidth,
        nHeight
    );
}
