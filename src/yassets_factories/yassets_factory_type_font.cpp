#include <yassets_factory_type_font.h>
#include <yasset_text.h>

// ---------------------------------------------------------------------
// YAssetFactoryFont

YAssetFactoryFont::YAssetFactoryFont(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeFont *pFactoryTypeFont,
    const std::string &sImagePath,
    const std::string &sAlphabet
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = "YAssetFactoryFont";
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
    TAG = "YAssetFactoryTypeFont";
}

std::string YAssetFactoryTypeFont::getFabricTypeId() {
    return "font";
}

YAssetFactory *YAssetFactoryTypeFont::createFactory(
    const std::string &sAssetFactoryPath,
    const std::string &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::string sImagePath = sAssetFactoryPath + "/" + jsonFactoryConfig["font-image"].getString();
    if (!YCore::fileExists(sImagePath)) {
        YLog::throw_err(TAG, "Did not font-image " + sImagePath);
    }
    std::string sAlphabet = jsonFactoryConfig["font-alphabet"].getString();

    return new YAssetFactoryFont(
        m_pAssetsService, 
        this,
        sImagePath,
        sAlphabet
    );
}
