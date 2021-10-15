
#include "settings_yservice.h"
// #include <algorithm>
#include <ycore.h>

// ---------------------------------------------------------------------
// SettingsYService

REGISTRY_YSERVICE(SettingsYService)


SettingsYService::SettingsYService()
    : YServiceBase(SettingsYService::name(), {}) {

    TAG = SettingsYService::name();
}

bool SettingsYService::init() {
    // checking settings
    // YLog::info(TAG, "init");
    if (!findResourceDir()) {
        return false;
    }
    return true;
}

bool SettingsYService::deinit() {
    // checking settings
    YLog::info(TAG, "deinit");
    m_sStringMap.clear();
    return true;
}

void SettingsYService::set(const std::string &sName, const std::string &sValue) {
    m_sStringMap[sName] = sValue;
}

bool SettingsYService::has(const std::string &sName) {
    return m_sStringMap.find(sName) != m_sStringMap.end();
}

std::string SettingsYService::get(const std::string &sName) {
    if (m_sStringMap.find(sName) != m_sStringMap.end()) {
        return m_sStringMap[sName];
    }
    return "";
}

std::string SettingsYService::getResourceDir() {
    return m_sResourceDir;
}

bool SettingsYService::findResourceDir() {
    std::vector<std::string> vPaths;
    vPaths.push_back("/usr/share/yourCityIsInvadedByAliens");
    vPaths.push_back("./res");
    m_sResourceDir = "";
    for (int i = 0; i < vPaths.size(); i++) {
        if (YCore::dirExists(vPaths[i])) {
            m_sResourceDir = vPaths[i];
            break;
        }
    }
    if (m_sResourceDir == "") {
        std::cerr << "Not found resource dir (./res by default)" << std::endl;
        return false;
    }
    YLog::info(TAG, "Resource dir: " + m_sResourceDir);
    return true;
}
