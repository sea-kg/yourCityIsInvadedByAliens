#include <yassets_factory_type_font.h>
#include <yasset_text.h>

// ---------------------------------------------------------------------
// YAssetFactoryFont

YAssetFactoryFont::YAssetFactoryFont(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeFont *pFactoryTypeFont,
    const std::wstring &sImagePath,
    const std::wstring &sAlphabet
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryFont";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sImagePath = sImagePath;
    m_sAlphabet = sAlphabet;
    // TODO: Not working now because render window now initialized to this moment
    m_pTexture = pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
}

YAsset *YAssetFactoryFont::createAsset() {
    // TODO: ad-hoc - time loading textures wrong, before then created render window
    m_pTexture = m_pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
    return new YAssetText(
        m_pAssetsService,
        m_pTexture,
        m_sAlphabet
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeFont

YAssetFactoryTypeFont::YAssetFactoryTypeFont(YAssetsService *pAssetsService) 
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeFont";
}

std::wstring YAssetFactoryTypeFont::getFabricTypeId() {
    return L"font";
}

YAssetFactory *YAssetFactoryTypeFont::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sImagePath = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"font-image"].getString();
    if (!YCore::fileExists(sImagePath)) {
        YLog::throw_err(TAG, L"Did not font-image " + sImagePath);
    }
    std::wstring sAlphabet = jsonFactoryConfig[L"font-alphabet"].getString();

    return new YAssetFactoryFont(
        m_pAssetsService, 
        this,
        sImagePath,
        sAlphabet
    );
}
