#pragma once
#include <string>
#include <memory>

#include "yjson.h"
#include "ypos.h"

class ConfigDefaultMap {
    public:
        ConfigDefaultMap(const YJson &jsonDefaultMap);
        const int getMaxClouds() const;
        const int getMaxBuildings() const;
        const int getWidth() const;
        const int getHeight() const;
        const YPos &getPlayerStartPos() const;
        const std::wstring &getBackgroundAssetId() const;
        const int getBackgroundPaddingTextureCount() const;
        const YPos &getMinPointMap() const;
        const YPos &getMaxPointMap() const;

        static std::shared_ptr<ConfigDefaultMap> loadConfig(const std::wstring &sDefaultPath);

    private:
        const int m_nMapWidth;
        const int m_nMapHeight;
        const int m_nMaxClouds;
        const int m_nMaxBuildings;
        const YPos m_playerStartPos;
        const std::wstring m_sBackgroundAssetId;
        const int m_nBackgroundPaddingTextureCount;
        const YPos m_minPointMap;
        const YPos m_maxPointMap;
};
