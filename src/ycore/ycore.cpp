#include "ycore.h"
#ifndef _WIN32
    #include <dirent.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <arpa/inet.h>
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
    #ifndef _WIN32
        DIR *dir = opendir(ws2s(sDirname).c_str());
        if (dir != NULL) {
            struct dirent *entry = readdir(dir);
            while (entry != NULL) {
                if (entry->d_type == DT_DIR) {
                    std::string sDir(entry->d_name);
                    if (sDir != "." && sDir != "..") {
                        vDirs.push_back(s2ws(sDir));
                    }
                }
                entry = readdir(dir);
            }
            closedir(dir);
        }
        std::sort(vDirs.begin(), vDirs.end());
    #else
        // TODO
    #endif
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
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string YCore::ws2s(const std::wstring& wstr) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}