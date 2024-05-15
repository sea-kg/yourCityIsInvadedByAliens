#include "yassets_factory_type_tank.h"
#include "yasset_tank.h"

// ---------------------------------------------------------------------
// YAssetFactoryTank

YAssetFactoryTank::YAssetFactoryTank(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeTank *pFactoryTypeTank,
    const std::wstring &sSpriteTankPath,
    const std::wstring &sSpriteRocketPath,
    int nFrameWidth,
    int nFrameHeight
) : YAssetFactory(pAssetsService, pFactoryTypeTank) {
    TAG = L"YAssetFactoryTank";
    m_pFactoryTypeTank = pFactoryTypeTank;
    m_sSpriteTankPath = sSpriteTankPath;
    m_sSpriteRocketPath = sSpriteRocketPath;
    m_nFrameWidth = nFrameWidth;
    m_nFrameHeight = nFrameHeight;
    m_pTextureTank = loadTexture(m_sSpriteTankPath);
    m_pTextureRocket = loadTexture(m_sSpriteRocketPath);
}

std::unique_ptr<YAsset> YAssetFactoryTank::createAsset() {
    return std::make_unique<YAssetTank>(
        m_pAssetsService,
        m_pTextureTank,
        m_pTextureRocket,
        m_nFrameWidth,
        m_nFrameHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeTank

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeTank)

YAssetFactoryTypeTank::YAssetFactoryTypeTank(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeTank";
}

const std::wstring &YAssetFactoryTypeTank::getFactoryTypeId() {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeTank::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sSpriteTankPath = jsonFactoryConfig[L"sprite-tank"].getString();
    sSpriteTankPath = sAssetFactoryPath + L"/" + sSpriteTankPath;
    if (!YCore::fileExists(sSpriteTankPath)) {
        YLog::throw_err(TAG, L"Did not image " + sSpriteTankPath);
    }

    std::wstring sSpriteRocketPath = jsonFactoryConfig[L"sprite-rocket"].getString();
    sSpriteRocketPath = sAssetFactoryPath + L"/" + sSpriteRocketPath;
    if (!YCore::fileExists(sSpriteRocketPath)) {
        YLog::throw_err(TAG, L"Did not image " + sSpriteRocketPath);
    }

    int nWidth = jsonFactoryConfig[L"frame-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"frame-height"].getNumber();

    return new YAssetFactoryTank(
        m_pAssetsService, 
        this,
        sSpriteTankPath,
        sSpriteRocketPath,
        nWidth,
        nHeight
    );
}
