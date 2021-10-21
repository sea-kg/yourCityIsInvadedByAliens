#pragma once
#include <string>

class YLog {
    public:
        static void info(const std::wstring &sTag, const std::wstring &sMessage);
        static void ok(const std::wstring &sTag, const std::wstring &sMessage);
        static void err(const std::wstring &sTag, const std::wstring &sMessage);
        static void throw_err(const std::wstring &sTag, const std::wstring &sMessage);
};