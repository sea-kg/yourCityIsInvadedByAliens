#include "ycore.h"
#ifndef _WIN32
    #include <dirent.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    #pragma warning(push)
    #pragma warning(disable: 4995)
    #include <AtlBase.h>
    #include <atlconv.h>
    #pragma warning(pop)
#else
    #include <direct.h>
    #define PATH_MAX 256
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <codecvt>
#include <sstream>
#include <locale>
#include <filesystem>
#include <iostream>

// ---------------------------------------------------------------------
// YCore

std::wstring YCore::getCurrentDirectory() {
    char cwd[PATH_MAX];
    #ifndef _WIN32
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            YLog::throw_err(L"getCurrentDirectory", L"Could not get current directory");
        }
    #else
        if (_getcwd(cwd, sizeof(cwd)) == NULL) {
            YLog::throw_err(L"getCurrentDirectory", L"Could not get current directory");
        }
    #endif
    return s2ws(std::string(cwd) + "/");
}

bool YCore::fileExists(const std::wstring &sFilename) {
    struct stat st;
    bool bExists = (stat(ws2s(sFilename).c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

bool YCore::dirExists(const std::wstring &sDirname) {
    struct stat st;
    bool bExists = (stat(ws2s(sDirname).c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

std::vector<std::wstring> YCore::getListOfDirs(const std::wstring &sDirname) {
    std::vector<std::wstring> vDirs;
    if (!YCore::dirExists(sDirname)) {
        return vDirs;
    }
    for (auto& entry : std::filesystem::directory_iterator(sDirname)) {
        if (entry.is_directory()) {
            // fix windows
            std::wstring sPath = YCore::s2ws(entry.path().string());
            // std::wcout << sPath << std::endl;
            sPath = sPath.substr(sDirname.size());
            // std::wcout << sPath << std::endl;
            vDirs.push_back(sPath);
        }
    }
    std::sort(vDirs.begin(), vDirs.end());
    return vDirs;
}

std::vector<std::wstring> YCore::getListOfFiles(const std::wstring &sDirname) {
    std::vector<std::wstring> vFiles;
    if (!YCore::dirExists(sDirname)) {
        return vFiles;
    }
    #ifndef _WIN32
        DIR *dir = opendir(ws2s(sDirname).c_str());
        if (dir != NULL) {
            struct dirent *entry = readdir(dir);
            while (entry != NULL) {
                if (entry->d_type != DT_DIR) {
                    std::string sDir(entry->d_name);
                    if (sDir != "." && sDir != "..") {
                        vFiles.push_back(s2ws(sDir));
                    }
                }
                entry = readdir(dir);
            }
            closedir(dir);
        }
    #else
        
    #endif
    return vFiles;
}

std::wstring YCore::join(const std::vector<std::wstring> &vWhat, const std::wstring& sDelim) {
    std::wstring sRet;
    for (unsigned int i = 0; i < vWhat.size(); i++) {
        if (i != 0) {
            sRet += sDelim;
        }
        sRet += vWhat[i];
    }
    return sRet;
}

std::wstring YCore::s2ws(const std::string& str) {
    #if _WIN32
        std::wstringstream cls;
        cls << str.c_str();
        return cls.str();
    #else
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.from_bytes(str);
    #endif
}

std::string YCore::ws2s(const std::wstring& wstr) {
    #if _WIN32
        //setup converter
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;

        //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
        std::string converted_str = converter.to_bytes( wstr );
        return converted_str;
    #else
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.to_bytes(wstr);
    #endif
}

// ---------------------------------------------------------------------
// YCore

YNeedUpdate::YNeedUpdate() {
    m_bNeedUpdated = true;
}

void YNeedUpdate::setYes() {
    m_bNeedUpdated = true;
}

bool YNeedUpdate::isAndDoReset() {
    if (m_bNeedUpdated) {
        m_bNeedUpdated = false;
        return true;
    }
    return false;
}
