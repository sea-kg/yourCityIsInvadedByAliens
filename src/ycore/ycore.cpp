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

// ---------------------------------------------------------------------
// YCore

std::string YCore::getCurrentDirectory() {
    char cwd[PATH_MAX];
    #ifndef _WIN32
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            YLog::throw_err("getCurrentDirectory", "Could not get current directory");
        }
    #else 
        if (_getcwd(cwd, sizeof(cwd)) == NULL) {
            YLog::throw_err("getCurrentDirectory", "Could not get current directory");
        }
    #endif
    return std::string(cwd) + "/";
}

bool YCore::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

bool YCore::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

std::vector<std::string> YCore::getListOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!YCore::dirExists(sDirname)) {
        return vDirs;
    }
    #ifndef _WIN32
        DIR *dir = opendir(sDirname.c_str());
        if (dir != NULL) {
            struct dirent *entry = readdir(dir);
            while (entry != NULL) {
                if (entry->d_type == DT_DIR) {
                    std::string sDir(entry->d_name);
                    if (sDir != "." && sDir != "..") {
                        vDirs.push_back(sDir);
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

std::vector<std::string> YCore::getListOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!YCore::dirExists(sDirname)) {
        return vFiles;
    }
    #ifndef _WIN32
        DIR *dir = opendir(sDirname.c_str());
        if (dir != NULL) {
            struct dirent *entry = readdir(dir);
            while (entry != NULL) {
                if (entry->d_type != DT_DIR) {
                    std::string sDir(entry->d_name);
                    if (sDir != "." && sDir != "..") {
                        vFiles.push_back(sDir);
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