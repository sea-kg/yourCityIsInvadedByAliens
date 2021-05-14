#pragma once
#include "ylog.h"
#include <vector>

class YCore {
    public:
        static std::string getCurrentDirectory();
        static bool dirExists(const std::string &sFilename);
        static bool fileExists(const std::string &sFilename);
        static std::vector<std::string> getListOfDirs(const std::string &sDirname);
        static std::vector<std::string> getListOfFiles(const std::string &sDirname);
};

