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

    // TODO: Not working now because render window now initialized to this moment
    m_pTexture = pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
}

YAsset *YAssetFactoryProgressBar::createAsset() {
    // TODO: ad-hoc - time loading textures wrong, before then created render window
    m_pTexture = m_pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
    return new YAssetProgressBar(
        m_pAssetsService,
        m_pTexture,
        m_nWidth,
        m_nHeight
    );
    return nullptr;
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeProgressBar

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeProgressBar)

YAssetFactoryTypeProgressBar::YAssetFactoryTypeProgressBar(YAssetsService *pAssetsService) 
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeProgressBar";
}

std::wstring YAssetFactoryTypeProgressBar::getFactoryTypeId() {
    return L"progressbar";
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
