#include "yassets_factory_type_progressbar.h"
#include "yasset_progressbar.h"

// ---------------------------------------------------------------------
// YAssetFactoryProgressBar

YAssetFactoryProgressBar::YAssetFactoryProgressBar(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeProgressBar *pFactoryTypeFont,
    const std::wstring &sImagePath,
    int nWidth,
    int nHeight
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryProgressBar";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sImagePath = sImagePath;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_pTexture = loadTexture(m_sImagePath);
}

std::unique_ptr<YAsset> YAssetFactoryProgressBar::createAsset() {
    return std::make_unique<YAssetProgressBar>(
        m_pAssetsService,
        m_pTexture,
        m_nWidth,
        m_nHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeProgressBar

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeProgressBar)

YAssetFactoryTypeProgressBar::YAssetFactoryTypeProgressBar(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeProgressBar";
}

const std::wstring &YAssetFactoryTypeProgressBar::getFactoryTypeId() const {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeProgressBar::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sImagePath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"progress-image"].getString();
    if (!YCore::fileExists(sImagePath)) {
        YLog::throw_err(TAG, L"Did not progress-image " + sImagePath);
    }
    int nWidth = jsonFactoryConfig[L"progress-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"progress-height"].getNumber();

    return new YAssetFactoryProgressBar(
        m_pAssetsService,
        this,
        sImagePath,
        nWidth,
        nHeight
    );
}
