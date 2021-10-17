#include "yassets_service.h"
#include <wsjcpp_core.h>
#include <algorithm>
#include <fstream>
#include <ycore.h>
#include <yjson.h>
#include <yassets_factory_type_font.h>
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
    registerFabricType(new YAssetFactoryTypeFont(this));
    m_pRenderWindow = findYService<WindowYService>()->getRenderWindow();
    return true;
}

bool YAssetsService::deinit() {
    // checking settings
    YLog::info(TAG, "deinit");
    return true;
}

RenderWindow *YAssetsService::getRenderWindow() {
    return m_pRenderWindow;
}

void YAssetsService::registerFabricType(YAssetFactoryType *pFactoryType) {
    std::string sFactoryTypeId = pFactoryType->getFabricTypeId();
    if (hasFabricType(sFactoryTypeId)) {
        YLog::throw_err(TAG, "Already registered fabric-type: " + sFactoryTypeId);
    }
    m_mapYAssetsFactoryTypes[sFactoryTypeId] = pFactoryType;
    YLog::ok(TAG, "registered fabric-type '" + sFactoryTypeId + "'");
}

bool YAssetsService::hasFabricType(const std::string &sFactoryTypeId) {
    return m_mapYAssetsFactoryTypes.find(sFactoryTypeId) != m_mapYAssetsFactoryTypes.end();
}

bool YAssetsService::loadAssetFactory(const std::string &sPath, std::string &sError) {
    auto *pSettings = findYService<SettingsYService>();
    std::string sResourceDir = pSettings->getResourceDir();

    std::string sAssetsInfoPath = sPath + "/asset-factory.json";
    if (!YCore::fileExists(sAssetsInfoPath)) {
        sError = "Did not find file " + sAssetsInfoPath;
        YLog::err(TAG, sError);
        return false;
    }
    
    YJson jsonAssetFactory(sAssetsInfoPath);
    if (jsonAssetFactory.isParserFailed()) {
        sError = "Could not parse asset-factory" + sAssetsInfoPath;
        YLog::err(TAG, sError);
        return false;
    }
    
    std::string sAssetFactoryType = jsonAssetFactory["asset-faсtory-type"].getString();
    if (!hasFabricType(sAssetFactoryType)) {
        sError = "Not found asset fabric type: " + sAssetFactoryType;
        YLog::err(TAG, sError);
        return false;
    }
    YAssetFactoryType* pFactoryType = m_mapYAssetsFactoryTypes[sAssetFactoryType];

    std::string sAssetFactoryId = jsonAssetFactory["asset-faсtory-id"].getString();
    if (m_mapYAssetsFactories.find(sAssetFactoryId) != m_mapYAssetsFactories.end()) {
        sError = "Already exists asset-factory-id: " + sAssetFactoryId;
        YLog::err(TAG, sError);
        return false;
    }
    m_mapYAssetsFactories[sAssetFactoryId] = pFactoryType->createFactory(
        sPath,
        sAssetFactoryId,
        jsonAssetFactory["asset-faсtory-config"]
    );

    return true;
}

YAsset *YAssetsService::createAsset(const std::string &sAssetFactoryId) {
    if (m_mapYAssetsFactories.find(sAssetFactoryId) == m_mapYAssetsFactories.end()) {
        YLog::throw_err(TAG, "Not found asset-factory-id: " + sAssetFactoryId);
    }
    return m_mapYAssetsFactories[sAssetFactoryId]->createAsset();
}
