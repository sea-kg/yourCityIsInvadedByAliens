#ifndef WSJCPP_CORE_H
#define WSJCPP_CORE_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <deque>
#include <iostream>

class WsjcppCore {
    public:
        static bool init(
            int argc, char** argv, 
            const std::string &sApplicationName,
            const std::string &sApplicationVersion,
            const std::string &sApplicationAuthor,
            const std::string &sLibraryNameForExports
        );

        static std::string doNormalizePath(const std::string &sPath);
        static std::string extractFilename(const std::string &sPath);

        static long getCurrentTimeInMilliseconds();
        static long getCurrentTimeInSeconds();
        static std::string getCurrentTimeForFilename();

        // static bool makeDir(const std::string &sDirname);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);
        static bool readTextFile(const std::string &sFilename, std::string &sOutputContent);
        static bool readFileToBuffer(const std::string &sFilename, char *pBuffer[], int &nBufferSize);
        static bool writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize);
        static bool removeFile(const std::string &sFilename);
        static bool copyFile(const std::string &sSourceFilename, const std::string &sTargetFilename);

        static bool createEmptyFile(const std::string &sFilename);

        static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string toLower(const std::string &str);
        static std::string toUpper(const std::string& str);
        static void replaceAll(std::string& str, const std::string& from, const std::string& to);
        static std::vector<std::string> split(const std::string& sWhat, const std::string& sDelim);
        static std::string join(const std::vector<std::string> &vWhat, const std::string& sDelim);

        static void initRandom();
        static std::string createUuid();
        static std::string uint2hexString(unsigned int n);
        static unsigned long convertVoidToULong(void *p);
        static std::string getPointerAsHex(void *p);
        static std::string extractURLProtocol(const std::string& sValue);
        static bool getEnv(const std::string& sName, std::string& sValue);

        static std::string encodeUriComponent(const std::string& sValue);

        static std::string getHumanSizeBytes(long nBytes);

        static std::string doPadLeft(const std::string& sIn, char cWhat, int nLength);
        static std::string doPadRight(const std::string& sIn, char cWhat, int nLength);

};

#endif // WSJCPP_CORE_H


