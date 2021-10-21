#pragma once
#include "ylog.h"
#include <vector>

class YCore {
    public:
        static std::wstring getCurrentDirectory();
        static bool dirExists(const std::wstring &sFilename);
        static bool fileExists(const std::wstring &sFilename);
        static std::vector<std::wstring> getListOfDirs(const std::wstring &sDirname);
        static std::vector<std::wstring> getListOfFiles(const std::wstring &sDirname);
        static std::wstring join(const std::vector<std::wstring> &vWhat, const std::wstring& sDelim);
        static std::wstring s2ws(const std::string& str);
        static std::string ws2s(const std::wstring& wstr);
};

