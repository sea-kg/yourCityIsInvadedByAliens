#pragma once

#include <yservices.h>

#include <wsjcpp_core.h>
#include <string>
#include <map>
#include <fstream>
#include <ylog.h>
#include <ycore.h>
#include <yjson.h>

class YAssetsService;

class YAsset {
    friend class YAssetsService;
    public:
        YAsset(YAssetsService *pAssetsService);
        virtual ~YAsset();

    private:
        YAssetsService *m_pAssetsService;
};

class YAssetFactory;

class YAssetFactoryType {
    friend class YAssetsService;
    public:
        YAssetFactoryType(YAssetsService *pAssetsService);
        virtual std::string getFabricTypeId() = 0;
        virtual YAssetFactory *createFactory(
            const std::string &sAssetFactoryPath,
            const std::string &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) = 0;

    protected:
        std::string TAG;
        YAssetsService *m_pAssetsService;
};

class YAssetFactory {
    // friend class YAssetsService;
    public:
        YAssetFactory(YAssetsService *pAssetsService, YAssetFactoryType *pFactoryType);
    protected:
        std::string TAG;
    private:
        YAssetsService *m_pAssetsService;
        YAssetFactoryType *m_pFactoryType;
};

class YAssetsService : public YServiceBase {
    friend class YAsset;

    public:
        YAssetsService();
        static std::string name() { return "YAssetsService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        void registerFabricType(YAssetFactoryType *);
        bool hasFabricType(const std::string &sFactoryTypeId);
        bool loadAsset(const std::string &sPath, std::string &sRetError);

    private:
        std::string TAG;
        std::map<std::string, YAssetFactoryType*> m_mapYAssetsFactoryTypes;
        std::map<std::string, YAssetFactory*> m_mapYAssetsFactories;
};

/*

class YAsset {
    friend class YAssetsService;

    public:
        YAsset(YAssetsService *pTree, uint16_t nType);
        virtual ~YAsset();
        uint16_t getType();

        // virtual zero methods will be deny create basic class
        virtual bool writeDataPartToFile(std::ofstream &f, std::string &sError) = 0;
        virtual bool readDataPartFromFile(std::ifstream &f, std::string &sError) = 0;

        virtual std::string toString(const std::string &sIntent = "") = 0;

        YAsset *getParent();
        uint32_t getId();
        const std::vector<YAsset *> &getChilds();
        bool hasChilds();

    protected:
        void setParent(YAsset *pParent);
        void addChild(YAsset *pChild);
        void setId(uint32_t id);

    private:
        YAssetsService *m_pTree;
        YAsset *m_pParent;
        uint32_t m_nId; // it uniq identified node in a list
        std::vector<YAsset *> m_vChilds;
        uint16_t m_nType;
};

class IFabricAsset {
    public:
        virtual ~IFabricAsset() {};
        virtual YAsset *create() = 0;
};

template<class T>
class FabricWsjcppObjTreeNode : public IFabricAsset {
    public:
        FabricWsjcppObjTreeNode(YAssetsService *pTree) : m_pTree(pTree) { };
        ~FabricWsjcppObjTreeNode() {};
        virtual YAsset *create() override {
            return new T(m_pTree);
        };
    private:
        YAssetsService *m_pTree;
};

class WsjcppObjTreeChainDeclare {
    public:
        WsjcppObjTreeChainDeclare(YAssetsService *pTree);
        
        WsjcppObjTreeChainDeclare &addNode(YAsset *);

        template<class T, class A>
        WsjcppObjTreeChainDeclare &addT(const A &val) {
            YAsset *pNode = new T(m_pTree, val);
            return addNode(pNode);
        }
        template<typename T> WsjcppObjTreeChainDeclare &add(const T &val);
        template<typename T> WsjcppObjTreeChainDeclare &add(const T *val);
        WsjcppObjTreeChainDeclare &up();
        WsjcppObjTreeChainDeclare &switchTo(YAsset *);

    private:
        std::string TAG;
        YAssetsService *m_pTree;
        YAsset *m_pCurrentNode;
};

// define for inline templates
#define WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE( type0, classname ) \
    template<> inline \
    WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::add< type0 >(type0 const &val) { \
        return this->addT< classname >(val); \
    }

#define WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE_P( type0, classname ) \
    template<> inline \
    WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::add< type0 >(type0 const *val) { \
        return this->addT< classname >(val); \
    }

class WsjcppObjTreeNodeString : public YAsset {
    public:
        WsjcppObjTreeNodeString(YAssetsService *pTree, const std::string &sValue = "");
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_STRING; }; // string
        
        std::string getValue();
        void setValue(const std::string &sValue);
        
        // YAsset
        virtual bool writeDataPartToFile(std::ofstream &f, std::string &sError) override;
        virtual bool readDataPartFromFile(std::ifstream &f, std::string &sError) override;
        virtual std::string toString(const std::string &sIntent = "") override;

    private:
        std::string m_sValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(std::string, WsjcppObjTreeNodeString)
WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE_P(char, WsjcppObjTreeNodeString)

class WsjcppObjTreeNodeInteger : public YAsset {
    public:
        WsjcppObjTreeNodeInteger(YAssetsService *pTree, const int32_t &nValue = 0);
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_INTEGER; }; // integer 32
        
        int32_t getValue();
        void setValue(int32_t nValue);

        // YAsset
        virtual bool writeDataPartToFile(std::ofstream &f, std::string &sError) override;
        virtual bool readDataPartFromFile(std::ifstream &f, std::string &sError) override;
        virtual std::string toString(const std::string &sIntent = "") override;

    private:
        int32_t m_nValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(int, WsjcppObjTreeNodeInteger)

class WsjcppObjTreeNodeFloat : public YAsset {
    public:
        WsjcppObjTreeNodeFloat(YAssetsService *pTree, const float &nValue = 0);
        static uint16_t staticType() { return WSJCPP_OBJ_TREE_NODE_FLOAT; }; // float
        
        float getValue();
        void setValue(float nValue);

        // YAsset
        virtual bool writeDataPartToFile(std::ofstream &f, std::string &sError) override;
        virtual bool readDataPartFromFile(std::ifstream &f, std::string &sError) override;
        virtual std::string toString(const std::string &sIntent = "") override;

    private:
        float m_nValue;
};

WSJCPP_OBJ_TREE_CHAIN_DECLARE_INLINE(float, WsjcppObjTreeNodeFloat)

*/



