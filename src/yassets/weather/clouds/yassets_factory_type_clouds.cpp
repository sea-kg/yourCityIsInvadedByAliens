#include "yassets_factory_type_clouds.h"
#include "yasset_clouds.h"

// ---------------------------------------------------------------------
// YAssetFactoryClouds

YAssetFactoryClouds::YAssetFactoryClouds(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeClouds *pFactoryTypeFont,
    const std::vector<std::wstring> &vImages,
    int nWidth,
    int nHeight
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryClouds";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_vImages = vImages;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    for (int i = 0; i < m_vImages.size(); i++) {
        SDL_Texture *pTexture = loadTexture(m_vImages[i]);
        m_vTextures.push_back(pTexture);
    }
}

YAsset *YAssetFactoryClouds::createAsset() {
    int nCloudType = std::rand() % m_vTextures.size();
    return new YAssetClouds(
        m_pAssetsService,
        m_vTextures[nCloudType],
        m_nWidth,
        m_nHeight
    );
    return nullptr;
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeClouds

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeClouds)

YAssetFactoryTypeClouds::YAssetFactoryTypeClouds(YAssetsService *pAssetsService) 
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeClouds";
}

std::wstring YAssetFactoryTypeClouds::getFactoryTypeId() {
    return L"clouds";
}

YAssetFactory *YAssetFactoryTypeClouds::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    YJsonObject images = jsonFactoryConfig[L"images"];
    std::vector<std::wstring> vImages;
    for (int i = 0; i < images.length(); i++) {
        std::wstring sImagePath = sAssetFactoryPath + L"/" + images[i].getString();
        vImages.push_back(sImagePath);
        if (!YCore::fileExists(sImagePath)) {
            YLog::throw_err(TAG, L"Did not image " + sImagePath);
        }
    }

    int nWidth = jsonFactoryConfig[L"image-width"].getNumber();
    int nHeight = jsonFactoryConfig[L"image-height"].getNumber();

    return new YAssetFactoryClouds(
        m_pAssetsService, 
        this,
        vImages,
        nWidth,
        nHeight
    );
}
