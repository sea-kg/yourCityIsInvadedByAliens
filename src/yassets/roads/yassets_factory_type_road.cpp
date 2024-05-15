#include "yassets_factory_type_road.h"
#include "yasset_road.h"

// ---------------------------------------------------------------------
// YAssetFactoryRoad

YAssetFactoryRoad::YAssetFactoryRoad(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeRoad *pFactoryTypeRoad,
    const std::wstring &sRoadImageTilesPath,
    int nFrameWidth,
    int nFrameHeight
) : YAssetFactory(pAssetsService, pFactoryTypeRoad) {
    TAG = L"YAssetFactoryRoad";
    m_pFactoryTypeRoad = pFactoryTypeRoad;
    m_sRoadImageTilesPath = sRoadImageTilesPath;
    m_nFrameWidth = nFrameWidth;
    m_nFrameHeight = nFrameHeight;
    m_pTextureRoad = loadTexture(m_sRoadImageTilesPath);
}

std::unique_ptr<YAsset> YAssetFactoryRoad::createAsset() {
    return std::make_unique<YAssetRoad>(
        m_pAssetsService,
        m_pTextureRoad,
        m_nFrameWidth,
        m_nFrameHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeRoad

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeRoad)

YAssetFactoryTypeRoad::YAssetFactoryTypeRoad(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeRoad";
}

const std::wstring &YAssetFactoryTypeRoad::getFactoryTypeId() {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeRoad::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sRoadImageTilesPath = jsonFactoryConfig[L"image"].getString();
    sRoadImageTilesPath = sAssetFactoryPath + L"/" + sRoadImageTilesPath;
    if (!YCore::fileExists(sRoadImageTilesPath)) {
        YLog::throw_err(TAG, L"Did not image " + sRoadImageTilesPath);
    }

    int nWidth = jsonFactoryConfig[L"frame-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"frame-height"].getNumber();

    return new YAssetFactoryRoad(
        m_pAssetsService,
        this,
        sRoadImageTilesPath,
        nWidth,
        nHeight
    );
}
