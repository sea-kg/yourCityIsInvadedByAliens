#include "ylog.h"

#include <iostream>

// ---------------------------------------------------------------------
// YLog

void YLog::info(const std::string &sTag, const std::string &sMessage) {
    std::string sLogMessage = "INFO: <" + sTag + "> " + sMessage;
    std::cout << sLogMessage << std::endl;
}

void YLog::err(const std::string &sTag, const std::string &sMessage) {
    std::string sLogMessage = "ERROR: <" + sTag + "> " + sMessage;
    std::cout << sLogMessage << std::endl;
}

void YLog::throw_err(const std::string &sTag, const std::string &sMessage) {
    std::string sLogMessage = "CRITICAL ERROR: <" + sTag + "> " + sMessage;
    std::cout << sLogMessage << std::endl;
    throw std::runtime_error(sLogMessage);
}