
#include "settings_yservice.h"
#include <iostream>
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
    // YLog::info(TAG, L"init");
    if (!findResourceDir()) {
        return false;
    }
    return true;
}

bool SettingsYService::deinit() {
    // checking settings
    YLog::info(TAG, L"deinit");
    m_sStringMap.clear();
    return true;
}

void SettingsYService::set(const std::wstring &sName, const std::wstring &sValue) {
    m_sStringMap[sName] = sValue;
}

bool SettingsYService::has(const std::wstring &sName) {
    return m_sStringMap.find(sName) != m_sStringMap.end();
}

std::wstring SettingsYService::get(const std::wstring &sName) {
    if (m_sStringMap.find(sName) != m_sStringMap.end()) {
        return m_sStringMap[sName];
    }
    return L"";
}

std::wstring SettingsYService::getResourceDir() {
    return m_sResourceDir;
}

bool SettingsYService::findResourceDir() {
    std::vector<std::wstring> vPaths;
    vPaths.push_back(L"/usr/share/yourCityIsInvadedByAliens");
    vPaths.push_back(L"./res");
    m_sResourceDir = L"";
    for (int i = 0; i < vPaths.size(); i++) {
        if (YCore::dirExists(vPaths[i])) {
            m_sResourceDir = vPaths[i];
            break;
        }
    }
    if (m_sResourceDir == L"") {
        std::wcerr << L"Not found resource dir (./res by default)" << std::endl;
        return false;
    }
    YLog::info(TAG, L"Resource dir: " + m_sResourceDir);
    return true;
}
