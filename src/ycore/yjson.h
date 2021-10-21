#pragma once
#include <string>
#include <map>
#include <vector>

enum class YJsonObjectType {
    UNDEFINED,
    STRING,
    NUMBER,
    OBJECT,
    ARRAY
};

class YJsonObject {
    public:
        YJsonObject();
        ~YJsonObject();
        bool isUndefined();

        bool isString();
        void doString();
        std::wstring getString() const;
        void setString(std::wstring sValue);

        bool isNumber();
        void doNumber();
        int getNumber() const;
        void setNumber(int nValue);

        bool isObject();
        void doObject();
        std::vector<std::wstring> getKeys() const;
        const YJsonObject &operator[](const std::wstring &sName) const;
        void addKeyValue(const std::wstring &sKey, YJsonObject *pValue);
        bool containsKey(const std::wstring &sKey) const;

        bool isArray() const;
        void doArray();
        void push(YJsonObject *pValue);
        int length() const;
        const YJsonObject &operator[](int nIndex) const;

    private:
        void reset();
        std::wstring TAG;
        YJsonObjectType m_nType;
        std::wstring m_sValue;
        int m_nValue;
        std::map<std::wstring, YJsonObject *> m_mapObjects;
        std::vector<YJsonObject *> m_arrObjects;
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
    START_VALUE_STRING_ESCAPING,
    START_VALUE_NUMBER,
    START_VALUES_ARRAY,
    END_VALUE_NUMBER,
    END
};

class YJson {
    public:
        YJson();
        YJson(const std::wstring &sFilename);
        bool isParserFailed();
        std::vector<std::wstring> getKeys() const;
        const YJsonObject &operator[](const std::wstring &sName) const;

    private:
        bool toParse(const std::wstring &sLine);
        bool isSkipChar(char c);
        void pushObjectToStack(YJsonObject *);
        void popObjectFromStack();
        YJsonObject *getLastObjectFromStack();
        void printParserError(const std::wstring &sDescription);
        int m_nLineNumber;
        std::wstring m_sLineParse;
        std::wstring m_sFilename;
        bool m_bParserFailed;
        YJsonParserState m_nParserState;
        YJsonObject m_root;
        bool m_bStartRoot;
        std::vector<YJsonObject *> m_vParserStack;
        std::wstring sParseKeyName;
        std::wstring sParseKeyValue;
        std::wstring m_sValue;
};