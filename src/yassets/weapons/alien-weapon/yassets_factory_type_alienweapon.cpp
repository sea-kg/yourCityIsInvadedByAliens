#include "yassets_factory_type_alienweapon.h"
#include "yasset_alienweapon.h"

// ---------------------------------------------------------------------
// YAssetFactoryAlienWeapon

YAssetFactoryAlienWeapon::YAssetFactoryAlienWeapon(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeAlienWeapon *pFactoryType,
    const std::wstring &sSpritePath,
    int nFrameWidth,
    int nFrameHeight,
    int nFrameNumber
) : YAssetFactory(pAssetsService, pFactoryType) {
    TAG = L"YAssetFactoryAlienWeapon";
    m_pFactoryType = pFactoryType;
    m_sSpritePath = sSpritePath;
    m_pTexture = loadTexture(sSpritePath);
    m_nFrameWidth = nFrameWidth;
    m_nFrameHeight = nFrameHeight;
    m_nFrameNumber = nFrameNumber;
}

std::unique_ptr<YAsset> YAssetFactoryAlienWeapon::createAsset() {
    return std::make_unique<YAssetAlienWeapon>(
        m_pAssetsService,
        m_pTexture,
        m_nFrameWidth,
        m_nFrameHeight,
        m_nFrameNumber
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeAlienWeapon

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeAlienWeapon)

YAssetFactoryTypeAlienWeapon::YAssetFactoryTypeAlienWeapon(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeAlienWeapon";
}

const std::wstring &YAssetFactoryTypeAlienWeapon::getFactoryTypeId() {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeAlienWeapon::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sSpritePath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"sprite"].getString();
    if (!YCore::fileExists(sSpritePath)) {
        YLog::throw_err(TAG, L"Did not find sprite " + sSpritePath);
    }

    int nFrameWidth = jsonFactoryConfig[L"frame-width"].getNumber();
    int nFrameHeight = jsonFactoryConfig[L"frame-height"].getNumber();
    int nFrameNumber = jsonFactoryConfig[L"frame-number"].getNumber();

    return new YAssetFactoryAlienWeapon(
        m_pAssetsService,
        this,
        sSpritePath,
        nFrameWidth,
        nFrameHeight,
        nFrameNumber
    );
}
