#include "yassets_factory_type_image.h"
#include "yasset_image.h"

// ---------------------------------------------------------------------
// YAssetFactoryImage

YAssetFactoryImage::YAssetFactoryImage(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeImage *pFactoryTypeFont,
    const std::wstring &sImagePath,
    int nWidth,
    int nHeight
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryImage";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sImagePath = sImagePath;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_pTexture = loadTexture(m_sImagePath);
}

std::unique_ptr<YAsset> YAssetFactoryImage::createAsset() {
    return std::make_unique<YAssetImage>(
        m_pAssetsService,
        m_pTexture,
        m_nWidth,
        m_nHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeImage

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeImage)

YAssetFactoryTypeImage::YAssetFactoryTypeImage(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeImage";
}

const std::wstring &YAssetFactoryTypeImage::getFactoryTypeId() const {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeImage::createFactory(
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

    return new YAssetFactoryImage(
        m_pAssetsService,
        this,
        sImagePath,
        nWidth,
        nHeight
    );
}
