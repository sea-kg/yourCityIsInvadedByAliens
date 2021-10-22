#include <yassets_factory_type_font50x50.h>
#include <yasset_text.h>

// ---------------------------------------------------------------------
// YAssetFactoryFont50x50

YAssetFactoryFont50x50::YAssetFactoryFont50x50(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeFont50x50 *pFactoryTypeFont,
    const std::wstring &sImagePath,
    const std::vector<std::wstring> &vAlphabets
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryFont50x50";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sImagePath = sImagePath;
    m_vAlphabets = vAlphabets;
    // TODO: Not working now because render window now initialized to this moment
    m_pTexture = pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
}

YAsset *YAssetFactoryFont50x50::createAsset() {
    // TODO: ad-hoc - time loading textures wrong, before then created render window
    m_pTexture = m_pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
    return new YAssetText(
        m_pAssetsService,
        m_pTexture,
        m_vAlphabets
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeFont50x50

YAssetFactoryTypeFont50x50::YAssetFactoryTypeFont50x50(YAssetsService *pAssetsService) 
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeFont50x50";
}

std::wstring YAssetFactoryTypeFont50x50::getFabricTypeId() {
    return L"font50x50";
}

YAssetFactory *YAssetFactoryTypeFont50x50::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sImagePath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"font-image"].getString();
    if (!YCore::fileExists(sImagePath)) {
        YLog::throw_err(TAG, L"Did not font-image " + sImagePath);
    }
    std::vector<std::wstring> vAlphabets;
    YJsonObject obj = jsonFactoryConfig[L"font-alphabets"];
    for (int i = 0; i < obj.length(); i++) {
        vAlphabets.push_back(obj[i].getString());
    }

    return new YAssetFactoryFont50x50(
        m_pAssetsService, 
        this,
        sImagePath,
        vAlphabets
    );
}
