#pragma once

#include "yservices.h"

// ---------------------------------------------------------------------
// SettingsYService

class SettingsYService : public YServiceBase {
    public:
        SettingsYService();
        static std::string name() { return "SettingsYService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        void set(const std::string &sName, const std::string &sValue);
        bool has(const std::string &sName);
        std::string get(const std::string &sName);
        std::string getResourceDir();

    private:
        bool findResourceDir();

        std::string TAG;
        std::map<std::string, std::string> m_sStringMap;
        std::string m_sResourceDir;
};