#include "yassets_service.h"
#include <algorithm>
#include <fstream>
#include <ycore.h>
#include <yjson.h>
#include <yassets_factory_type_font50x50.h>
#include <settings_yservice.h>
#include <window_yservice.h>

// ---------------------------------------------------------------------
// YAsset

YAsset::YAsset(YAssetsService *pAssetsService) {
    m_pAssetsService = pAssetsService;
}

// virtual RenderObject *YAsset::getRenderObject() = 0;

// ---------------------------------------------------------------------
// YAssetFactoryType

YAssetFactoryType::YAssetFactoryType(YAssetsService *pAssetsService) {
    m_pAssetsService = pAssetsService;
}

// ---------------------------------------------------------------------
// YAssetFactory

YAssetFactory::YAssetFactory(YAssetsService *pAssetsService, YAssetFactoryType *pFactoryType) {
    m_pAssetsService = pAssetsService;
    m_pFactoryType = pFactoryType;
}

// ---------------------------------------------------------------------
// YAssetsService

REGISTRY_YSERVICE(YAssetsService)

YAssetsService::YAssetsService()
    : YServiceBase(YAssetsService::name(), {
        SettingsYService::name(),
        WindowYService::name()
    }) {
    TAG = YAssetsService::name();
    m_pRenderWindow = nullptr;
}

bool YAssetsService::init() {
    // refistration of factory types
    registerFabricType(new YAssetFactoryTypeFont50x50(this));
    m_pRenderWindow = findYService<WindowYService>()->getRenderWindow();
    return true;
}

bool YAssetsService::deinit() {
    // checking settings
    YLog::info(TAG, L"deinit");
    return true;
}

RenderWindow *YAssetsService::getRenderWindow() {
    return m_pRenderWindow;
}

void YAssetsService::registerFabricType(YAssetFactoryType *pFactoryType) {
    std::wstring sFactoryTypeId = pFactoryType->getFabricTypeId();
    if (hasFabricType(sFactoryTypeId)) {
        YLog::throw_err(TAG, L"Already registered fabric-type: " + sFactoryTypeId);
    }
    m_mapYAssetsFactoryTypes[sFactoryTypeId] = pFactoryType;
    YLog::ok(TAG, L"registered fabric-type '" + sFactoryTypeId + L"'");
}

bool YAssetsService::hasFabricType(const std::wstring &sFactoryTypeId) {
    return m_mapYAssetsFactoryTypes.find(sFactoryTypeId) != m_mapYAssetsFactoryTypes.end();
}

bool YAssetsService::loadAssetFactory(const std::wstring &sPath, std::wstring &sError) {
    auto *pSettings = findYService<SettingsYService>();
    std::wstring sResourceDir = pSettings->getResourceDir();

    std::wstring sAssetsInfoPath = sPath + L"/asset-factory.json";
    if (!YCore::fileExists(sAssetsInfoPath)) {
        sError = L"Did not find file " + sAssetsInfoPath;
        YLog::err(TAG, sError);
        return false;
    }
    
    YJson jsonAssetFactory(sAssetsInfoPath);
    if (jsonAssetFactory.isParserFailed()) {
        sError = L"Could not parse asset-factory" + sAssetsInfoPath;
        YLog::err(TAG, sError);
        return false;
    }
    
    std::wstring sAssetFactoryType = jsonAssetFactory[L"asset-faсtory-type"].getString();
    if (!hasFabricType(sAssetFactoryType)) {
        sError = L"Not found asset fabric type: " + sAssetFactoryType;
        YLog::err(TAG, sError);
        return false;
    }
    YAssetFactoryType* pFactoryType = m_mapYAssetsFactoryTypes[sAssetFactoryType];

    std::wstring sAssetFactoryId = jsonAssetFactory[L"asset-faсtory-id"].getString();
    if (m_mapYAssetsFactories.find(sAssetFactoryId) != m_mapYAssetsFactories.end()) {
        sError = L"Already exists asset-factory-id: " + sAssetFactoryId;
        YLog::err(TAG, sError);
        return false;
    }
    m_mapYAssetsFactories[sAssetFactoryId] = pFactoryType->createFactory(
        sPath,
        sAssetFactoryId,
        jsonAssetFactory[L"asset-faсtory-config"]
    );

    return true;
}

YAsset *YAssetsService::createAsset(const std::wstring &sAssetFactoryId) {
    if (m_mapYAssetsFactories.find(sAssetFactoryId) == m_mapYAssetsFactories.end()) {
        YLog::throw_err(TAG, L"Not found asset-factory-id: " + sAssetFactoryId);
    }
    return m_mapYAssetsFactories[sAssetFactoryId]->createAsset();
}
