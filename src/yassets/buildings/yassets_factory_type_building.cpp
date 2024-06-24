#include "yassets_factory_type_building.h"
#include "yasset_building.h"

// ---------------------------------------------------------------------
// YAssetFactoryBuilding

YAssetFactoryBuilding::YAssetFactoryBuilding(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeBuilding *pFactoryTypeBuilding,
    const std::wstring &sBuildingImagePath,
    int nWidth,
    int nHeight
) : YAssetFactory(pAssetsService, pFactoryTypeBuilding) {
    TAG = L"YAssetFactoryBuilding";
    m_pFactoryTypeBuilding = pFactoryTypeBuilding;
    m_sBuildingImagePath = sBuildingImagePath;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_pTextureBuilding = loadTexture(sBuildingImagePath);
}

std::unique_ptr<YAsset> YAssetFactoryBuilding::createAsset() {
    return std::make_unique<YAssetBuilding>(
        m_pAssetsService,
        m_pTextureBuilding,
        m_nWidth,
        m_nHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeBuilding

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeBuilding)

YAssetFactoryTypeBuilding::YAssetFactoryTypeBuilding(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeBuilding";
}

const std::wstring &YAssetFactoryTypeBuilding::getFactoryTypeId() {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeBuilding::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sBuildingImagePath = jsonFactoryConfig[L"image"].getString();
    sBuildingImagePath = sAssetFactoryPath + L"/" + sBuildingImagePath;
    if (!YCore::fileExists(sBuildingImagePath)) {
        YLog::throw_err(TAG, L"Did not image " + sBuildingImagePath);
    }

    int nWidth = jsonFactoryConfig[L"width"].getNumber();
    int nHeight = jsonFactoryConfig[L"height"].getNumber();

    return new YAssetFactoryBuilding(
        m_pAssetsService,
        this,
        sBuildingImagePath,
        nWidth,
        nHeight
    );
}
