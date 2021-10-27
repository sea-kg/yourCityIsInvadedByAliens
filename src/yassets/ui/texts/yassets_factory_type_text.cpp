#include "yassets_factory_type_text.h"
#include "yasset_text.h"

// ---------------------------------------------------------------------
// YAssetFactoryText

YAssetFactoryText::YAssetFactoryText(
    YAssetsService *pAssetsService,
    YAssetFactoryTypeText *pFactoryTypeFont,
    const std::wstring &sImagePath,
    const std::vector<std::wstring> &vAlphabets,
    int nLetterHeight,
    int nLetterWidth
) : YAssetFactory(pAssetsService, pFactoryTypeFont) {
    TAG = L"YAssetFactoryText";
    m_pFactoryTypeFont = pFactoryTypeFont;
    m_sImagePath = sImagePath;
    m_vAlphabets = vAlphabets;
    // TODO: Not working now because render window now initialized to this moment
    m_pTexture = pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
    m_nLetterHeight = nLetterHeight;
    m_nLetterWidth = nLetterWidth;
}

YAsset *YAssetFactoryText::createAsset() {
    // TODO: ad-hoc - time loading textures wrong, before then created render window
    m_pTexture = m_pAssetsService->getRenderWindow()->loadTexture(m_sImagePath);
    return new YAssetText(
        m_pAssetsService,
        m_pTexture,
        m_vAlphabets,
        m_nLetterHeight,
        m_nLetterWidth
    );
}

// ---------------------------------------------------------------------
// YAssetFactoryTypeText

REGISTRY_YASSET_FACTORY_TYPE(YAssetFactoryTypeText)

YAssetFactoryTypeText::YAssetFactoryTypeText(YAssetsService *pAssetsService) 
    : YAssetFactoryType(pAssetsService) {
    TAG = L"YAssetFactoryTypeText";
}

std::wstring YAssetFactoryTypeText::getFactoryTypeId() {
    return L"text";
}

YAssetFactory *YAssetFactoryTypeText::createFactory(
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

    int nLetterHeight = jsonFactoryConfig[L"letter-height"].getNumber();
    int nLetterWidth = jsonFactoryConfig[L"letter-width"].getNumber();
    return new YAssetFactoryText(
        m_pAssetsService, 
        this,
        sImagePath,
        vAlphabets,
        nLetterHeight,
        nLetterWidth
    );
}
