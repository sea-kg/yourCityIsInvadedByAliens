#include "yjson.h"
#include "ylog.h"
#include <iostream>
#include <fstream>

// ---------------------------------------------------------------------
// YJson

YJsonObject::YJsonObject() {
    TAG = "YJsonObject";
    m_nType = YJsonObjectType::UNDEFINED;
}

YJsonObject::~YJsonObject() {
    reset();
}

bool YJsonObject::isUndefined() {
    return m_nType == YJsonObjectType::UNDEFINED;
}

bool YJsonObject::isString() {
    return m_nType == YJsonObjectType::STRING;
}

void YJsonObject::doString() {
    if (m_nType != YJsonObjectType::UNDEFINED) {
        YLog::throw_err(TAG, "::doString(), expected UNDEFINED");
    }
    reset();
    m_nType = YJsonObjectType::STRING;
}

std::string YJsonObject::getString() const {
    if (m_nType != YJsonObjectType::STRING) {
        YLog::throw_err(TAG, "::getString, expected STRING");
    }
    return m_sValue;
}

void YJsonObject::setString(std::string sValue) {
    if (m_nType != YJsonObjectType::STRING) {
        YLog::throw_err(TAG, "::setString(), must be STRING");
    }
    m_sValue = sValue;
}

bool YJsonObject::isNumber() {
    return m_nType == YJsonObjectType::NUMBER;
}

void YJsonObject::doNumber() {
    if (m_nType != YJsonObjectType::UNDEFINED) {
        YLog::throw_err(TAG, "::doNumber(), must be UNDEFINED");
    }
    reset();
    m_nType = YJsonObjectType::NUMBER;
}

int YJsonObject::getNumber() const {
    if (m_nType != YJsonObjectType::NUMBER) {
        YLog::throw_err(TAG, "::getNumber, expected NUMBER");
    }
    return m_nValue;
}

void YJsonObject::setNumber(int nValue) {
    if (m_nType != YJsonObjectType::NUMBER) {
        YLog::throw_err(TAG, "::setNumber(), must be NUMBER");
    }
    m_nValue = nValue;
}

bool YJsonObject::isObject() {
    return m_nType == YJsonObjectType::OBJECT;
}

void YJsonObject::doObject() {
    if (m_nType != YJsonObjectType::UNDEFINED) {
        YLog::throw_err(TAG, "::doObject(), must be undefined");
    }
    reset();
    m_nType = YJsonObjectType::OBJECT;
}

std::vector<std::string> YJsonObject::getKeys() const {
    // TODO find the easy solution for get keys from map
    std::vector<std::string> vKeys;
    std::map<std::string, YJsonObject *>::const_iterator it = m_mapObjects.cbegin();
    for (it; it != m_mapObjects.end(); it++) {
        vKeys.push_back(it->first);
    }
    return vKeys;
}

const YJsonObject &YJsonObject::operator[](const std::string &sName) const {
    if (m_nType != YJsonObjectType::OBJECT) {
        YLog::throw_err(TAG, "::operator[], must be OBJECT");
    }
    std::map<std::string, YJsonObject *>::const_iterator it = m_mapObjects.find(sName);
    YJsonObject *pObject = nullptr;
    if (it != m_mapObjects.end()) {
        pObject = it->second;
    } else {
        YLog::throw_err(TAG, "[" + sName + "] - not found");
    }
    return *pObject;
}

void YJsonObject::addKeyValue(const std::string &sKey, YJsonObject *pValue) {
    if (m_nType != YJsonObjectType::OBJECT) {
        YLog::throw_err(TAG, "::addKeyValue(), must be OBJECT");
    }
    m_mapObjects.insert(std::pair<std::string, YJsonObject*>(sKey,pValue));
}

bool YJsonObject::containsKey(const std::string &sKey) const {
    if (m_nType != YJsonObjectType::OBJECT) {
        YLog::throw_err(TAG, "::containsKey(), must be OBJECT");
    }
    std::map<std::string, YJsonObject *>::const_iterator it = m_mapObjects.find(sKey);
    return it != m_mapObjects.end();
}

bool YJsonObject::isArray() const {
    return m_nType == YJsonObjectType::ARRAY;
}

void YJsonObject::doArray() {
    if (m_nType != YJsonObjectType::UNDEFINED) {
        YLog::throw_err(TAG, "::doArray, must be undefined");
    }
    reset();
    m_nType = YJsonObjectType::ARRAY;
}

void YJsonObject::push(YJsonObject *pValue) {
    if (m_nType != YJsonObjectType::ARRAY) {
        YLog::throw_err(TAG, "::addArrayItem, expected array");
    }
    m_arrObjects.push_back(pValue);
}

int YJsonObject::length() const {
    if (m_nType != YJsonObjectType::ARRAY) {
        YLog::throw_err(TAG, "::length, expected array");
    }
    return m_arrObjects.size();
}

const YJsonObject &YJsonObject::operator[](int nIndex) const {
    YJsonObject *pObject = nullptr;
    if (nIndex < m_arrObjects.size()) {
        pObject = m_arrObjects[nIndex];
    } else {
        YLog::throw_err(TAG, "[" + std::to_string(nIndex) + "] - not found");
    }
    return *pObject;
}


void YJsonObject::reset() {
    m_nType = YJsonObjectType::UNDEFINED;
    m_sValue = "";
    m_nValue = 0;
    std::map<std::string, YJsonObject *>::iterator it = m_mapObjects.begin();
    for (it; it != m_mapObjects.begin(); it--) {
        YJsonObject *pObject = it->second;
        delete pObject;
        it->second = nullptr;
    }
    m_mapObjects.clear();
    for (int i = 0; i < m_arrObjects.size(); i++) {
        YJsonObject *pObject = m_arrObjects[i];
        delete pObject;
        m_arrObjects[i] = nullptr;
    }
    m_arrObjects.clear();
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
    m_root.doObject();
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
            YJsonObject *pPrevObject = getLastObjectFromStack();
            bool bArray = getLastObjectFromStack()->isArray();
            if (m_bStartRoot) {
                m_bStartRoot = false;
            } else {
                YJsonObject *pValue = new YJsonObject();
                pushObjectToStack(pValue);
            }
            if (pPrevObject->isObject()) {
                m_nParserState = YJsonParserState::START_KEY;
            } else if (pPrevObject->isArray()) {
                m_nParserState = YJsonParserState::START_VALUES_ARRAY;
            } else {
                printParserError("START_OBJECT, unknown what a do");
            }
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
                // std::cout << " START_KEY_NAME -> END_KEY " << std::endl;
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
            // std::cout << " END_KEY -> START_VALUE " << std::endl;
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
                // std::cout << "START_VALUE -> START_VALUE_STRING " << std::endl;
                m_nParserState = YJsonParserState::START_VALUE_STRING;
                if (getLastObjectFromStack()->isUndefined()) {
                    getLastObjectFromStack()->doString();
                }
                if (!getLastObjectFromStack()->isString()) {
                    printParserError("START_VALUE/START_VALUE_STRING Expected string");
                    return false;
                }
                sParseKeyValue = "";
                continue;    
            } else if (c == '{') {
                // std::cout << "START_VALUE -> START_VALUE_OBJECT " << std::endl;
                m_nParserState = YJsonParserState::START_VALUE_OBJECT;
                if (getLastObjectFromStack()->isUndefined()) {
                    getLastObjectFromStack()->doObject();
                }
                if (!getLastObjectFromStack()->isObject()) {
                    printParserError("START_VALUE/START_VALUE_OBJECT Expected object");
                    return false;
                }
                continue;
            } else if (c == '[') {
                // std::cout << "START_VALUE -> START_VALUES_ARRAY " << std::endl;
                m_nParserState = YJsonParserState::START_VALUES_ARRAY;
                if (getLastObjectFromStack()->isUndefined()) {
                    getLastObjectFromStack()->doArray();
                }
                continue;
            } else if ((c >= '0' && c <= '9') || c == '-') {
                m_nParserState = YJsonParserState::START_VALUE_NUMBER;
                if (getLastObjectFromStack()->isUndefined()) {
                    getLastObjectFromStack()->doNumber();
                }
                if (!getLastObjectFromStack()->isNumber()) {
                    printParserError("START_VALUE/START_VALUE_NUMBER Expected number");
                    return false;
                }
                sParseKeyValue = "";
                sParseKeyValue += c;
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
            // std::cout << " START_VALUE_OBJECT -> START_KEY_NAME " << std::endl;
            continue;
        }
        if (m_nParserState == YJsonParserState::START_VALUE_STRING_ESCAPING) {
            sParseKeyValue += c;
            m_nParserState = YJsonParserState::START_VALUE_STRING;
            continue;
        }
        if (m_nParserState == YJsonParserState::START_VALUE_STRING) {
            if (c == '\\') {
                m_nParserState = YJsonParserState::START_VALUE_STRING_ESCAPING;
                continue;
            } else if (c == '"') {
                if (getLastObjectFromStack()->isUndefined()) {
                    getLastObjectFromStack()->doString();
                }
                if (!getLastObjectFromStack()->isString()) {
                    printParserError("Expeceted string");
                    return false;
                }
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
            if (getLastObjectFromStack()->isUndefined()) {
                getLastObjectFromStack()->doNumber();
            }
            if (!getLastObjectFromStack()->isNumber()) {
                printParserError("Expeceted number");
                return false;
            }
            getLastObjectFromStack()->setNumber(std::stoi(sParseKeyValue));
            popObjectFromStack();
            m_nParserState = YJsonParserState::END_VALUE;
            i--;
            // std::cout << "number, sParseKeyValue: " << sParseKeyValue << std::endl;
            continue;
        }
        if (m_nParserState == YJsonParserState::START_VALUES_ARRAY) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c == '{') {
                YJsonObject *pValue = new YJsonObject();
                pValue->doObject();
                getLastObjectFromStack()->push(pValue);
                pushObjectToStack(pValue);
                m_nParserState = YJsonParserState::START_VALUE_OBJECT;
                // std::cout << " START_VALUES_ARRAY -> START_VALUE_OBJECT " << std::endl;
                continue;
            } /*else if (c == ',') {
                YJsonObject *pValue = new YJsonObject();
                getLastObjectFromStack()->push(pValue);
                pushObjectToStack(pValue);
                m_nParserState = YJsonParserState::START_VALUE;
                std::cout << "START_VALUES_ARRAY -> START_VALUE " << std::endl;
                continue;
            } else*/ {
                std::string sError = "end value, unexpected '";
                sError += c;
                sError += "'";
                printParserError(sError);
                return false;
            }
        } 
        
        if (m_nParserState == YJsonParserState::END_VALUE) {
            if (isSkipChar(c)) {
                continue;
            }
            if (c == ',') {
                if (getLastObjectFromStack()->isObject()) {
                    m_nParserState = YJsonParserState::START_KEY;
                } else if (getLastObjectFromStack()->isArray()) {
                    m_nParserState = YJsonParserState::START_VALUES_ARRAY;
                } else {
                    printParserError("END_VALUE, unknown what are do");
                }
                continue;
            } else if (c == '}') {
                popObjectFromStack();
                if (m_vParserStack.size() == 0) {
                    m_nParserState = YJsonParserState::END;
                    continue;
                }
                m_nParserState = YJsonParserState::END_VALUE;
                // std::cout << "END_VALUE - } -> END_VALUE " << std::endl;
                continue;
            } else if (c == ']') {
                popObjectFromStack();
                if (m_vParserStack.size() == 0) {
                    m_nParserState = YJsonParserState::END;
                    continue;
                }
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
        if (m_nParserState == YJsonParserState::END) {
            if (isSkipChar(c)) {
                continue;
            }
            std::string sError = "end value, unexpected '";
            sError += c;
            sError += "'";
            printParserError(sError);
            return false;
        }
    }
    return true;
}

bool YJson::isSkipChar(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void YJson::pushObjectToStack(YJsonObject *pObject) {
    // std::cout << "::pushObjectToStack" << std::endl;
    m_vParserStack.push_back(pObject);
}

void YJson::popObjectFromStack() {
    // std::cout << "::popObjectFromStack" << std::endl;
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
    std::string sPath = ""; 
    for (int i = 0; i < m_vParserStack.size(); i++) {
        YJsonObject *pObject = m_vParserStack[i];
        if (pObject->isUndefined()) {
            sPath += "/undefined";
        } else if (pObject->isString()) {
            sPath += "/string";
        } else if (pObject->isNumber()) {
            sPath += "/number";
        } else if (pObject->isObject()) {
            sPath += "/object";
        } else if (pObject->isArray()) {
            sPath += "/array";
        } else {
            sPath += "/wtf?";
        }
    }
    std::cout << "JSON-PARSER-ERROR: " << std::endl
        << "Size of ParserStack: " << m_vParserStack.size() << std::endl
        << "ParserStack: " << sPath << std::endl
        << "Filepos: " << m_sFilename << ":" << m_nLineNumber << std::endl
        << "Content: '" << m_sLineParse << "'" << std::endl
        << "Description: " << sDescription << std::endl
    ;
}