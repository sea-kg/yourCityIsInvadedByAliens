
#include <fstream>
#include <algorithm>
#include <iostream>
#include "config_default_map.h"
#include "ylog.h"


ConfigDefaultMap::ConfigDefaultMap(const YJson &jsonDefaultMap)
:
    m_nMapWidth(jsonDefaultMap[L"width"].getNumber()),
    m_nMapHeight(jsonDefaultMap[L"height"].getNumber()),
    m_nMaxClouds(jsonDefaultMap[L"max-clouds"].getNumber()),
    m_nMaxBuildings(jsonDefaultMap[L"max-buildings"].getNumber()),
    m_playerStartPos(jsonDefaultMap[L"player-start-x"].getNumber(), jsonDefaultMap[L"player-start-y"].getNumber()),
    m_sBackgroundAssetId(jsonDefaultMap[L"background-asset"].getString()),
    m_nBackgroundPaddingTextureCount(jsonDefaultMap[L"background-padding-texture-count"].getNumber()),
    m_minPointMap(YPos(0,0)),
    m_maxPointMap(YPos(m_nMapWidth, m_nMapHeight))
{
    // nothing
}

const int ConfigDefaultMap::getMaxClouds() const {
    return m_nMaxClouds;
}

const int ConfigDefaultMap::getMaxBuildings() const {
    return m_nMaxBuildings;
}

const int ConfigDefaultMap::getWidth() const {
    return m_nMapWidth;
}

const int ConfigDefaultMap::getHeight() const {
    return m_nMapHeight;
}

const YPos &ConfigDefaultMap::getPlayerStartPos() const {
    return m_playerStartPos;
}

const std::wstring &ConfigDefaultMap::getBackgroundAssetId() const {
    return m_sBackgroundAssetId;
}

const int ConfigDefaultMap::getBackgroundPaddingTextureCount() const {
    return m_nBackgroundPaddingTextureCount;
}

const YPos &ConfigDefaultMap::getMinPointMap() const {
    return m_minPointMap;
}

const YPos &ConfigDefaultMap::getMaxPointMap() const {
    return m_maxPointMap;
}

std::shared_ptr<ConfigDefaultMap> ConfigDefaultMap::loadConfig(const std::wstring &sDefaultPath) {
    std::wstring sPath(sDefaultPath + L"/map.json");
    std::cout << L"Deafult map path: " << sPath.c_str() << std::endl;
    YJson jsonDefaultMap(sPath);
    if (jsonDefaultMap.isParserFailed()) {
        YLog::throw_err(L"ConfigDefaultMap::loadConfig", L"Could not read or parse: " + sPath);
    }
    return std::make_shared<ConfigDefaultMap>(jsonDefaultMap);
}
