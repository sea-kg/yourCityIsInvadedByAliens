#pragma once

#include "yservices.h"

// ---------------------------------------------------------------------
// SettingsYService

class SettingsYService : public YServiceBase {
    public:
        SettingsYService();
        static std::wstring name() { return L"SettingsYService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        void set(const std::wstring &sName, const std::wstring &sValue);
        bool has(const std::wstring &sName);
        std::wstring get(const std::wstring &sName);
        std::wstring getResourceDir();

    private:
        bool findResourceDir();

        std::wstring TAG;
        std::map<std::wstring, std::wstring> m_sStringMap;
        std::wstring m_sResourceDir;
};