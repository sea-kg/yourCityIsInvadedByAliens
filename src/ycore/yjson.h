#pragma once
#include <string>
#include <map>
#include <vector>

class YJsonObject {
    public:
        YJsonObject();
        bool isUndefined();
        

        bool isString();
        std::string getString() const;
        void setString(std::string sValue);

        bool isNumber();
        int getNumber() const;
        void setNumber(int nValue);

        bool isObject();
        std::vector<std::string> getKeys() const;
        const YJsonObject &operator[](const std::string &sName) const;
        void addKeyValue(const std::string &sKey, YJsonObject *pValue);

    private:
        bool m_bUndefined;
        std::string m_sValue;
        int m_nValue;
        bool m_bString;
        bool m_bNumber;
        bool m_bObject;

        std::map<std::string, YJsonObject *> m_mapObjects;
};

enum class YJsonParserState {
    START_KEY,
    START_KEY_NAME,
    END_KEY,
    START_OBJECT,
    START_VALUE,
    END_VALUE,
    START_VALUE_OBJECT,
    START_VALUE_STRING,
    START_VALUE_NUMBER,
    END_VALUE_NUMBER,
    END
};

class YJson {
    public:
        YJson();
        YJson(const std::string &sFilename);
        bool isParserFailed();
        std::string getString() const;
        int getNumber() const;
        std::vector<std::string> getKeys() const;
        const YJsonObject &operator[](const std::string &sName) const;

    private:
        bool toParse(const std::string &sLine);
        bool isSkipChar(char c);
        void pushObjectToStack(YJsonObject *);
        void popObjectFromStack();
        YJsonObject *getLastObjectFromStack();
        void printParserError(const std::string &sDescription);
        int m_nLineNumber;
        std::string m_sLineParse;
        std::string m_sFilename;
        bool m_bParserFailed;
        YJsonParserState m_nParserState;
        YJsonObject m_root;
        bool m_bStartRoot;
        std::vector<YJsonObject *> m_vParserStack;
        std::string sParseKeyName;
        std::string sParseKeyValue;
        std::string m_sValue;
};