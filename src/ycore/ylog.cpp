#include "ylog.h"

#include <iostream>
#include <codecvt>
#include <sstream>
#include <locale>

// ---------------------------------------------------------------------
// YLog

void YLog::info(const std::wstring &sTag, const std::wstring &sMessage) {
    std::wstring sLogMessage = L"INFO: <" + sTag + L"> " + sMessage;
    std::wcout << sLogMessage << std::endl;
}

void YLog::ok(const std::wstring &sTag, const std::wstring &sMessage) {
    std::wstring sLogMessage = L"OK: <" + sTag + L"> " + sMessage;
    std::wcout << sLogMessage << std::endl;
}

void YLog::err(const std::wstring &sTag, const std::wstring &sMessage) {
    std::wstring sLogMessage = L"ERROR: <" + sTag + L"> " + sMessage;
    std::wcout << sLogMessage << std::endl;
}

void YLog::throw_err(const std::wstring &sTag, const std::wstring &sMessage) {
    std::wstring sLogMessage = L"CRITICAL ERROR: <" + sTag + L"> " + sMessage;
    std::wcout << sLogMessage << std::endl;
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    throw std::runtime_error(converterX.to_bytes(sLogMessage));
}
