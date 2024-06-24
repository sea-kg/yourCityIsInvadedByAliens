#include "yassets_factory_type_alien_berry.h"
#include "yasset_alien_berry.h"

// ---------------------------------------------------------------------
// YAssetFactoryAlienBerry

YAssetFactoryAlienBerry::YAssetFactoryAlienBerry(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeAlienBerry *pFactoryTypeAlienBerry,
    const std::wstring &sSpriteAlienBerryPath,
    int nFrameWidth,
    int nFrameHeight,
    int nFrameNumber
) : YAssetFactory(pAssetsService, pFactoryTypeAlienBerry) {
    TAG = L"YAssetFactoryAlienBerry";
    m_pFactoryTypeAlienBerry = pFactoryTypeAlienBerry;
    m_sSpriteAlienBerryPath = sSpriteAlienBerryPath;
    m_nFrameWidth = nFrameWidth;
    m_nFrameHeight = nFrameHeight;
    m_nFrameNumber = nFrameNumber;
    m_pTextureAlienBerry = loadTexture(m_sSpriteAlienBerryPath);
}

std::unique_ptr<YAsset> YAssetFactoryAlienBerry::createAsset() {
    return std::make_unique<YAssetAlienBerry>(
        m_pAssetsService,
        m_pTextureAlienBerry,
        m_nFrameWidth,
        m_nFrameHeight,
        m_nFrameNumber
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeAlienBerry

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeAlienBerry)

YAssetFactoryTypeAlienBerry::YAssetFactoryTypeAlienBerry(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeAlienBerry";
}

const std::wstring &YAssetFactoryTypeAlienBerry::getFactoryTypeId() {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeAlienBerry::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sSpriteAlienBerryPath = jsonFactoryConfig[L"sprite"].getString();
    sSpriteAlienBerryPath = sAssetFactoryPath + L"/" + sSpriteAlienBerryPath;
    if (!YCore::fileExists(sSpriteAlienBerryPath)) {
        YLog::throw_err(TAG, L"Did not image " + sSpriteAlienBerryPath);
    }

    int nFrameWidth = jsonFactoryConfig[L"frame-width"].getNumber();
    int nFrameHeight = jsonFactoryConfig[L"frame-height"].getNumber();
    int nFrameNumber = jsonFactoryConfig[L"frame-number"].getNumber();

    return new YAssetFactoryAlienBerry(
        m_pAssetsService,
        this,
        sSpriteAlienBerryPath,
        nFrameWidth,
        nFrameHeight,
        nFrameNumber
    );
}
