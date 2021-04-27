#include "yjson.h"
#include <iostream>
#include <fstream>

// ---------------------------------------------------------------------
// YJson

YJsonObject::YJsonObject() {
    m_bUndefined = true;
    m_bString = false;
    m_bNumber = false;
    m_bObject = false;
}

bool YJsonObject::isUndefined() {
    return m_bUndefined;
}

bool YJsonObject::isObject() {
    return m_bObject;
}

bool YJsonObject::isString() {
    return m_bString;
}

std::string YJsonObject::getString() const {
    return m_sValue;
}

void YJsonObject::setString(std::string sValue) {
    m_bUndefined = false;
    m_bString = true;
    m_sValue = sValue;
}

bool YJsonObject::isNumber() {
    return m_bNumber;
}

int YJsonObject::getNumber() const {
    return m_nValue;
}

void YJsonObject::setNumber(int nValue) {
    m_bUndefined = false;
    m_bNumber = true;
    m_nValue = nValue;
}

std::vector<std::string> YJsonObject::getKeys() const {
    std::vector<std::string> vKeys;
    std::map<std::string, YJsonObject *>::const_iterator it = m_mapObjects.cbegin();
    for (it; it != m_mapObjects.end(); it++) {
        vKeys.push_back(it->first);
    }
    return vKeys;
}

const YJsonObject &YJsonObject::operator[](const std::string &sName) const {
    std::map<std::string, YJsonObject *>::const_iterator it = m_mapObjects.find(sName);
    if (it != m_mapObjects.end()) {
        return *(it->second);
    }
    return YJsonObject();
}

void YJsonObject::addKeyValue(const std::string &sKey, YJsonObject *pValue) {
    m_mapObjects.insert(std::pair<std::string, YJsonObject*>(sKey,pValue));
}

// ---------------------------------------------------------------------
// YJson

YJson::YJson() {
    m_sValue = "";
}

YJson::YJson(const std::string &sFilename) {
    m_bParserFailed = false;
    m_sFilename = sFilename;
    m_nParserState = YJsonParserState::START_OBJECT;
    m_nLineNumber = 0;
    m_vParserStack.clear();
    pushObjectToStack(&m_root);
    m_bStartRoot = true;
    std::ifstream ifData(sFilename);
    if (ifData.is_open()) {
        std::string sLine;
        while (getline (ifData, sLine)) {
            if (!toParse(sLine)) {
                m_bParserFailed = true;
                break;
            }
        }
        ifData.close();
    }
}

bool YJson::isParserFailed() {
    return m_bParserFailed || m_nParserState != YJsonParserState::END;
}

std::string YJson::getString() const {
    return m_sValue;
}

int YJson::getNumber() const {
    // TODO  atoi (m_sValue)
    return 0;
}

std::vector<std::string> YJson::getKeys() const {
    return m_root.getKeys();
}

const YJsonObject &YJson::operator[](const std::string &sName) const {
    return m_root[sName];
}

bool YJson::toParse(const std::string &sLine) {
    // std::cout << sLine << std::endl;
    m_sLineParse = sLine;
    m_nLineNumber++;
    for (int i = 0; i < sLine.length(); i++) {
        char c = sLine[i];
        if (m_nParserState == YJsonParserState::START_OBJECT) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c != '{') {
                std::string sError = "start object, json expected '{', but got '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
            if (m_bStartRoot) {
                m_bStartRoot = false;
            } else {
                YJsonObject *pValue = new YJsonObject();
                pushObjectToStack(pValue);
            }
            m_nParserState = YJsonParserState::START_KEY;
            continue;
        }
        if (m_nParserState == YJsonParserState::START_KEY) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c != '"') {
                std::string sError = "start key, json expected '\"', but got '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
            sParseKeyName = "";
            m_nParserState = YJsonParserState::START_KEY_NAME;
            continue;
        }
        if (m_nParserState == YJsonParserState::START_KEY_NAME) {
            if (c == '"') {
                m_nParserState = YJsonParserState::END_KEY;
                // std::cout << "sParseKeyName: " << sParseKeyName << std::endl;
                continue;
            }
            sParseKeyName += c;
        }
        if (m_nParserState == YJsonParserState::END_KEY) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c != ':') {
                std::string sError = "end key name, json expected ':', but got '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
            m_nParserState = YJsonParserState::START_VALUE;
            YJsonObject *pValue = new YJsonObject();
            getLastObjectFromStack()->addKeyValue(sParseKeyName, pValue);
            pushObjectToStack(pValue);
            continue;
        }
        if (m_nParserState == YJsonParserState::START_VALUE) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c == '"') {
                m_nParserState = YJsonParserState::START_VALUE_STRING;
                sParseKeyValue = "";
                // std::cout << "START_VALUE_STRING" << std::endl;
                continue;    
            } else if (c == '{') {
                m_nParserState = YJsonParserState::START_VALUE_OBJECT;
                // std::cout << "START_VALUE_OBJECT" << std::endl;
                continue;
            } else if ((c >= '0' && c <= '9') || c == '-') {
                sParseKeyValue = "";
                sParseKeyValue += c;
                m_nParserState = YJsonParserState::START_VALUE_NUMBER;
                // std::cout << "START_VALUE_NUMBER" << std::endl;
                continue;
            } else {
                std::string sError = "start value, unexpected '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
        }
        if (m_nParserState == YJsonParserState::START_VALUE_OBJECT) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c != '"') {
                std::string sError = "start value object, json expected '\"', but got '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
            sParseKeyName = "";
            m_nParserState = YJsonParserState::START_KEY_NAME;
            continue;
        }
        if (m_nParserState == YJsonParserState::START_VALUE_STRING) {
            if (c == '"') {
                // std::cout << "string, sParseKeyValue: " << sParseKeyValue << std::endl;
                getLastObjectFromStack()->setString(sParseKeyValue);
                popObjectFromStack();
                m_nParserState = YJsonParserState::END_VALUE;
                continue;
            }
            sParseKeyValue += c;
        }
        if (m_nParserState == YJsonParserState::START_VALUE_NUMBER) {
            if (c >= '0' && c <= '9') {
                sParseKeyValue += c;
                continue;
            }
            getLastObjectFromStack()->setNumber(std::stoi(sParseKeyValue));
            popObjectFromStack();
            m_nParserState = YJsonParserState::END_VALUE;
            i--;
            // std::cout << "number, sParseKeyValue: " << sParseKeyValue << std::endl;
            continue;
        }
        if (m_nParserState == YJsonParserState::END_VALUE) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c == ',') {
                m_nParserState = YJsonParserState::START_KEY;
                // std::cout << " -> START_KEY " << std::endl;
                continue;
            } else if (c == '}') {
                popObjectFromStack();
                m_nParserState = YJsonParserState::END_VALUE;
                // std::cout << " -> END_VALUE " << std::endl;
                continue;
            } else {
                std::string sError = "end value, unexpected '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
        }
    }
    return true;
}

bool YJson::isSkipChar(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void YJson::pushObjectToStack(YJsonObject *pObject) {
    m_vParserStack.push_back(pObject);
}

void YJson::popObjectFromStack() {
    if (m_vParserStack.size() == 0) {
        std::cout << "ERROR: stack is empty()" << std::endl;
    }
    m_vParserStack.pop_back();
}

YJsonObject *YJson::getLastObjectFromStack() {
    if (m_vParserStack.size() == 0) {
        std::cout << "ERROR: stack is empty()" << std::endl;
        return nullptr;
    }
    return m_vParserStack[m_vParserStack.size() - 1];
}

void YJson::printParserError(const std::string &sDescription) {
    std::cout << "JSON-PARSER-ERROR: " << std::endl
        << "Filepos: " << m_sFilename << ":" << m_nLineNumber << std::endl
        << "Content: '" << m_sLineParse << "'" << std::endl
        << "Description: " << sDescription << std::endl
    ;
}