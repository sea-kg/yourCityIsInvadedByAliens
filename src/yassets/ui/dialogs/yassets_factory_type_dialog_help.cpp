#include "yassets_factory_type_dialog_help.h"
#include "yasset_dialog_help.h"

// ---------------------------------------------------------------------
// YAssetFactoryDialogHelp

YAssetFactoryDialogHelp::YAssetFactoryDialogHelp(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeDialogHelp *pFactoryTypeFont,
    const std::wstring &sImageHelpPath,
    int nHelpWidth,
    int nHelpHeight,
    const std::wstring &sImageDialogBackgroundPath,
    int nDialogBackgroundWidth,
    int nDialogBackgroundHeight
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryDialogHelp";
    m_pFactoryTypeFont = pFactoryTypeFont;

    m_sImageHelpPath = sImageHelpPath;
    m_pTextureHelp = loadTexture(m_sImageHelpPath);
    m_nHelpWidth = nHelpWidth;
    m_nHelpHeight = nHelpHeight;

    m_sImageDialogBackgroundPath = sImageDialogBackgroundPath;
    m_pTextureDialogBackground = loadTexture(m_sImageDialogBackgroundPath);
    m_nDialogBackgroundWidth = nDialogBackgroundWidth;
    m_nDialogBackgroundHeight = nDialogBackgroundHeight;
}

std::unique_ptr<YAsset> YAssetFactoryDialogHelp::createAsset() {
    return std::make_unique<YAssetDialogHelp>(
        m_pAssetsService,
        m_pTextureHelp,
        m_nHelpWidth,
        m_nHelpHeight,
        m_pTextureDialogBackground,
        m_nDialogBackgroundWidth,
        m_nDialogBackgroundHeight
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeDialogHelp

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeDialogHelp)

YAssetFactoryTypeDialogHelp::YAssetFactoryTypeDialogHelp(YAssetsService *pAssetsService)
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeDialogHelp";
}

const std::wstring &YAssetFactoryTypeDialogHelp::getFactoryTypeId() const {
    return m_sType;
}

YAssetFactory *YAssetFactoryTypeDialogHelp::createFactory(
    const std::wstring &sAssetFactoryPath,
    const std::wstring &sFactoryId,
    const YJsonObject &jsonFactoryConfig
) {
    std::wstring sImageHelp = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"image-content"].getString();
    if (!YCore::fileExists(sImageHelp)) {
        YLog::throw_err(TAG, L"Did not image " + sImageHelp);
    }
    int nHelpWidth = jsonFactoryConfig[L"image-content-width"].getNumber();
    int nHelpHeight = jsonFactoryConfig[L"image-content-height"].getNumber();

    std::wstring sImageDialogBackground = sAssetFactoryPath + L"/" + jsonFactoryConfig[L"image-background"].getString();
    if (!YCore::fileExists(sImageDialogBackground)) {
        YLog::throw_err(TAG, L"Did not image " + sImageDialogBackground);
    }
    int nDialogBackgroundWidth = jsonFactoryConfig[L"image-background-width"].getNumber();
    int nDialogBackgroundHeight = jsonFactoryConfig[L"image-background-height"].getNumber();

    return new YAssetFactoryDialogHelp(
        m_pAssetsService,
        this,
        sImageHelp,
        nHelpWidth,
        nHelpHeight,
        sImageDialogBackground,
        nDialogBackgroundWidth,
        nDialogBackgroundHeight
    );
}
