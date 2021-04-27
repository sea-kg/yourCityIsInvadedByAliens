#include "wsjcpp_core.h"
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
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <math.h>
#include <chrono>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <cstdint>
#include <streambuf>
#include <random>
#include <iomanip>


// ---------------------------------------------------------------------
// WsjcppCore

bool WsjcppCore::init(
    int argc, char** argv, 
    const std::string &sApplicationName,
    const std::string &sApplicationVersion,
    const std::string &sApplicationAuthor,
    const std::string &sLibraryNameForExports
) {
    // init random
    // std::srand(std::time(0));
    WsjcppCore::initRandom();
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::doNormalizePath(const std::string & sPath) {
    // split path by /
    std::vector<std::string> vNames;
    std::string s = "";
    int nStrLen = sPath.length();
    for (unsigned int i = 0; i < sPath.length(); i++) {
        if (sPath[i] == '/') {
            vNames.push_back(s);
            s = "";
            if (i == nStrLen-1) {
                vNames.push_back("");
            }
        } else {
            s += sPath[i];
        }
    }
    if (s != "") {
         vNames.push_back(s);
    }

    // fildered
    int nLen = vNames.size();
    std::vector<std::string> vNewNames;
    for (int i = 0; i < nLen; i++) {
        std::string sCurrent = vNames[i];
        if (sCurrent == "" && i == nLen-1) {
            vNewNames.push_back(sCurrent);
            continue;
        }

        if ((sCurrent == "" || sCurrent == ".") && i != 0) {
            continue;
        }

        if (sCurrent == ".." && vNewNames.size() > 0) {
            std::string sPrev = vNewNames[vNewNames.size()-1];
            if (sPrev == "") {
                vNewNames.pop_back();
                vNewNames.push_back(sCurrent);
            } else if (sPrev != "." && sPrev != "..") {
                vNewNames.pop_back();
            } else {
                vNewNames.push_back(sCurrent);
            }
        } else {
            vNewNames.push_back(sCurrent);
        }
    }
    std::string sRet = "";
    int nNewLen = vNewNames.size();
    int nLastNew = nNewLen-1;
    for (int i = 0; i < nNewLen; i++) {
        sRet += vNewNames[i];
        if (i != nLastNew) {
            sRet += "/";
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::extractFilename(const std::string &sPath) {
    // split path by /
    std::vector<std::string> vNames;
    std::string s = "";
    int nStrLen = sPath.length();
    for (unsigned int i = 0; i < sPath.length(); i++) {
        if (sPath[i] == '/') {
            vNames.push_back(s);
            s = "";
            if (i == nStrLen-1) {
                vNames.push_back("");
            }
        } else {
            s += sPath[i];
        }
    }
    if (s != "") {
         vNames.push_back(s);
    }
    std::string sRet;
    if (vNames.size() > 0) {
        sRet = vNames[vNames.size()-1];
    }
    return sRet;
}



// ---------------------------------------------------------------------

long WsjcppCore::getCurrentTimeInMilliseconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

long WsjcppCore::getCurrentTimeInSeconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

bool WsjcppCore::writeFile(const std::string &sFilename, const std::string &sContent) {
    
    // std::ofstream f(sFilename, std::ifstream::in);
    std::ofstream f(sFilename, std::ios::out);
    if (!f) {
        // WsjcppLog::err("WsjcppCore", "Could not create file to write '" + sFilename + "'");
        return false;
    }
    f << sContent << std::endl;
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::readTextFile(const std::string &sFilename, std::string &sContent) {
    
    std::ifstream f(sFilename);
    if (!f) {
        std::cout << "FAILED could not open file to read " << sFilename << std::endl;
        return false;
    }

    sContent = std::string(
        (std::istreambuf_iterator<char>(f)),
        std::istreambuf_iterator<char>()
    );

    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::readFileToBuffer(const std::string &sFilename, char *pBuffer[], int &nBufferSize) {
    std::ifstream f(sFilename, std::ifstream::binary);
    if (!f) {
        return false;
    }

    // get length of file:
    f.seekg (0, f.end);
    nBufferSize = f.tellg();
    f.seekg (0, f.beg);

    *pBuffer = new char [nBufferSize];

    // read data as a block:
    f.read (*pBuffer, nBufferSize);
    if (!f) {
        // WsjcppLog::err("WsjcppCore::readFileToBuffer", "Only " + std::to_string(f.gcount()) + " could be read");
        delete[] pBuffer;
        f.close();
        return false;
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppCore::writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize) {
    std::ofstream f(sFilename, std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to wtite " << sFilename << std::endl;
        return false;
    }
    f.write(pBuffer, nBufferSize);
    f.close();
    return true;
}

// ---------------------------------------------------------------------

std::string& WsjcppCore::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

// ---------------------------------------------------------------------

std::string& WsjcppCore::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

// ---------------------------------------------------------------------

std::string& WsjcppCore::trim(std::string& str, const std::string& chars) {
    return WsjcppCore::ltrim(WsjcppCore::rtrim(str, chars), chars);
}

// ---------------------------------------------------------------------
// will worked only with latin

std::string WsjcppCore::toLower(const std::string& str) {
    std::string sRet = str;
    std::transform(sRet.begin(), sRet.end(), sRet.begin(), ::tolower);
    return sRet;
}

// ---------------------------------------------------------------------
// will worked only with latin

std::string WsjcppCore::toUpper(const std::string& str) {
    std::string sRet = str;
    std::transform(sRet.begin(), sRet.end(), sRet.begin(), ::toupper);
    return sRet;
}

// ---------------------------------------------------------------------

void WsjcppCore::replaceAll(std::string& str, const std::string& sFrom, const std::string& sTo) {
    if (sFrom.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(sFrom, start_pos)) != std::string::npos) {
        str.replace(start_pos, sFrom.length(), sTo);
        start_pos += sTo.length(); // In case 'to' contains 'sFrom', like replacing 'x' with 'yx'
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppCore::split(const std::string& sWhat, const std::string& sDelim) {
    std::vector<std::string> vRet;
    int nPos = 0;
    int nLen = sWhat.length();
    int nDelimLen = sDelim.length();
    while (nPos < nLen) {
        std::size_t nFoundPos = sWhat.find(sDelim, nPos);
        if (nFoundPos != std::string::npos) {
            std::string sToken = sWhat.substr(nPos, nFoundPos - nPos);
            vRet.push_back(sToken);
            nPos = nFoundPos + nDelimLen;
            if (nFoundPos + nDelimLen == nLen) { // last delimiter
                vRet.push_back("");
            }
        } else {
            std::string sToken = sWhat.substr(nPos, nLen - nPos);
            vRet.push_back(sToken);
            break;
        }
    }
    return vRet;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::join(const std::vector<std::string> &vWhat, const std::string& sDelim) {
    std::string sRet;
    for (unsigned int i = 0; i < vWhat.size(); i++) {
        if (i != 0) {
            sRet += sDelim;
        }
        sRet += vWhat[i];
    }
    return sRet;
}

// ---------------------------------------------------------------------

void WsjcppCore::initRandom() {
    std::srand(std::time(0));
}

// ---------------------------------------------------------------------

std::string WsjcppCore::createUuid() {
    std::string sRet = "00000000-0000-0000-0000-000000000000";
    const std::string sAlphabet = "0123456789abcdef";
    for (int i = 0; i < 36; i++) {
        if (i != 8 && i != 13 && i != 18 && i != 23) {
            sRet[i] = sAlphabet[rand() % sAlphabet.length()];
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::uint2hexString(unsigned int n) {
    std::string sRet;
    for (int i = 0; i < 8; i++) {
        sRet += "0123456789abcdef"[n % 16];
        n >>= 4;
    }
    return std::string(sRet.rbegin(), sRet.rend());
}

// ---------------------------------------------------------------------

unsigned long WsjcppCore::convertVoidToULong(void *p) {
    std::uintptr_t ret = reinterpret_cast<std::uintptr_t>(p);
    return (unsigned long)ret;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getPointerAsHex(void *p) {
    std::uintptr_t i = reinterpret_cast<std::uintptr_t>(p);
    std::stringstream stream;
    stream << std::hex << i;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

std::string WsjcppCore::extractURLProtocol(const std::string& sValue) {
    std::string sRet = "";
    int nPosProtocol = sValue.find("://");
    if (nPosProtocol == std::string::npos) {
        return sRet;
    }
    sRet = sValue.substr(0, nPosProtocol);
    return sRet;
}

// ---------------------------------------------------------------------

bool WsjcppCore::getEnv(const std::string& sName, std::string& sValue) {
    if (const char* env_p = std::getenv(sName.c_str())) {
        sValue = std::string(env_p);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------

std::string WsjcppCore::encodeUriComponent(const std::string& sValue) {
    std::stringstream ssRet;
    for (unsigned int i = 0; i < sValue.length(); i++) {
        char c = sValue[i];
        if (
            c == '-' || c == '_' || c == '.' || c == '!'
            || c == '~' || c == '*' || c == '\'' 
            || c == '(' || c == ')' || (c >= '0' && c <= '9') 
            || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        ) {
            ssRet << c;
        } else {
            ssRet << '%' << std::uppercase << std::hex << (int)c;
        }
    }
    return ssRet.str();
}

// ---------------------------------------------------------------------

std::string WsjcppCore::getHumanSizeBytes(long nBytes) {
    if (nBytes == 0) {
        return "0B";
    }
    std::string arrPrefix[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    long n0 = nBytes;
    long n1 = 0;
    for (int i = 0; i < 6; i++) {
        if (n0 >= 1 && n0 < 1000) {
            return std::to_string(n0) + arrPrefix[i];
        }        
        n0 = nBytes / 1000;
        n1 = nBytes - n0 * 1000;
        n0 += n1 >= 500 ? 1 : 0;

        nBytes = nBytes / 1000;
        if (n0 == 0 && n1 == 0) {
            return "fuck"; 
        }
    }
    return std::to_string(nBytes) + "PB";
}

// ---------------------------------------------------------------------
/*
bool WsjcppCore::recoursiveCopyFiles(const std::string& sSourceDir, const std::string& sTargetDir) {
    if (!WsjcppCore::dirExists(sSourceDir)) {
        WsjcppLog::err("recoursiveCopyFiles", "Source Dir '" + sSourceDir + "' did not exists");
        return false;
    }

    if (!WsjcppCore::dirExists(sTargetDir)) {
        if (!WsjcppCore::makeDir(sTargetDir)) {
            WsjcppLog::err("recoursiveCopyFiles", "Could not create target dir '" + sTargetDir + "'");
            return false;
        }
    }

    std::vector<std::string> vFiles = WsjcppCore::getListOfFiles(sSourceDir);
    for (int i = 0; i < vFiles.size(); i++) {
        std::string sSourceFile = sSourceDir + "/" + vFiles[i];
        std::string sTargetFile = sTargetDir + "/" + vFiles[i];
        if (!WsjcppCore::copyFile(sSourceFile, sTargetFile)) {
            return false;
        }
    }

    std::vector<std::string> vDirs = WsjcppCore::getListOfDirs(sSourceDir);
    for (int i = 0; i < vDirs.size(); i++) {
        std::string sSourceDir2 = sSourceDir + "/" + vDirs[i];
        std::string sTargetDir2 = sTargetDir + "/" + vDirs[i];
        if (!WsjcppCore::dirExists(sTargetDir2)) {
            if (!WsjcppCore::makeDir(sTargetDir2)) {
                WsjcppLog::err("recoursiveCopyFiles", "Could not create target subdir '" + sTargetDir2 + "'");
                return false;
            }
        }

        if (!WsjcppCore::recoursiveCopyFiles(sSourceDir2, sTargetDir2)) {
            return false;
        }
    }
    return true;
}
*/

// ---------------------------------------------------------------------

std::string WsjcppCore::doPadLeft(const std::string& sIn, char cWhat, int nLength) {
    std::string sRet;
    int nPadLen = nLength - sIn.length();
    for (int i = 0; i < nPadLen; i++) {
        sRet += cWhat;
    }
    return sRet + sIn; 
}

// ---------------------------------------------------------------------

std::string WsjcppCore::doPadRight(const std::string& sIn, char cWhat, int nLength) {
    std::string sRet;
    int nPadLen = nLength - sIn.length();
    for (int i = 0; i < nPadLen; i++) {
        sRet += cWhat;
    }
    return sIn + sRet;
}
