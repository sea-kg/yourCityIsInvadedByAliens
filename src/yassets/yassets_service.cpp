#include "yassets_service.h"
#include <wsjcpp_core.h>
#include <algorithm>
#include <fstream>
#include <ycore.h>
#include <yjson.h>
#include <yassets_fabric_type_font.h>
#include <settings_yservice.h>

// ---------------------------------------------------------------------
// YAsset

YAsset::YAsset(YAssetsService *pAssetsService) {
    m_pAssetsService = pAssetsService;
}

YAsset::~YAsset() {

}

// ---------------------------------------------------------------------
// YAssetFactoryType

YAssetFactoryType::YAssetFactoryType(YAssetsService *pAssetsService) {
    m_pAssetsService = pAssetsService;
}

// ---------------------------------------------------------------------
// YAssetFactory

YAssetFactory::YAssetFactory(YAssetsService *pAssetsService, YAssetFactoryType *pFactoryType) {
    m_pAssetsService = pAssetsService;
    m_pFactoryType = pFactoryType;
}

// ---------------------------------------------------------------------
// YAssetsService

REGISTRY_YSERVICE(YAssetsService)

YAssetsService::YAssetsService()
    : YServiceBase(YAssetsService::name(), { SettingsYService::name() }) {
    TAG = YAssetsService::name();
}

bool YAssetsService::init() {
    // refistration of factory types
    registerFabricType(new YAssetFabricTypeFont(this));
    return true;
}

bool YAssetsService::deinit() {
    // checking settings
    YLog::info(TAG, "deinit");
    return true;
}

void YAssetsService::registerFabricType(YAssetFactoryType *pFactoryType) {
    std::string sFactoryTypeId = pFactoryType->getFabricTypeId();
    if (hasFabricType(sFactoryTypeId)) {
        YLog::throw_err(TAG, "Already registered fabric-type: " + sFactoryTypeId);
    }
    m_mapYAssetsFactoryTypes[sFactoryTypeId] = pFactoryType;
    YLog::ok(TAG, "registered fabric-type '" + sFactoryTypeId + "'");
}

bool YAssetsService::hasFabricType(const std::string &sFactoryTypeId) {
    return m_mapYAssetsFactoryTypes.find(sFactoryTypeId) != m_mapYAssetsFactoryTypes.end();
}

bool YAssetsService::loadAsset(const std::string &sPath, std::string &sError) {
    auto *pSettings = findYService<SettingsYService>();
    std::string sResourceDir = pSettings->getResourceDir();

    std::string sAssetsInfoPath = sPath + "/asset-factory.json";
    if (!YCore::fileExists(sAssetsInfoPath)) {
        sError = "Did not find file " + sAssetsInfoPath;
        YLog::err(TAG, sError);
        return false;
    }
    
    YJson jsonAssetFactory(sAssetsInfoPath);
    if (jsonAssetFactory.isParserFailed()) {
        sError = "Could not parse asset-factory" + sAssetsInfoPath;
        YLog::err(TAG, sError);
        return false;
    }
    
    std::string sAssetFactoryType = jsonAssetFactory["asset-faсtory-type"].getString();
    if (!hasFabricType(sAssetFactoryType)) {
        sError = "Not found asset fabric type: " + sAssetFactoryType;
        YLog::err(TAG, sError);
        return false;
    }
    YAssetFactoryType* pFactoryType = m_mapYAssetsFactoryTypes[sAssetFactoryType];

    std::string sAssetFactoryId = jsonAssetFactory["asset-faсtory-id"].getString();
    if (m_mapYAssetsFactories.find(sAssetFactoryId) != m_mapYAssetsFactories.end()) {
        sError = "Already exists asset-factory-id: " + sAssetFactoryId;
        YLog::err(TAG, sError);
        return false;
    }
    m_mapYAssetsFactories[sAssetFactoryId] = pFactoryType->createFactory(
        sPath,
        sAssetFactoryId,
        jsonAssetFactory["asset-faсtory-config"]
    );

    return true;
}

/*
// ---------------------------------------------------------------------
// YAsset

YAsset::YAsset(YAssetsService *pTree, uint16_t nType) {
    m_pTree = pTree;
    m_pParent = nullptr;
    m_nType = nType;
}

// ---------------------------------------------------------------------

uint16_t YAsset::getType() {
    return m_nType;
}

// ---------------------------------------------------------------------

YAsset::~YAsset() {
    // std::cout << "TreeNode destroyed" << std::endl;
}

// ---------------------------------------------------------------------

YAsset *YAsset::getParent() {
    return m_pParent;
}

// ---------------------------------------------------------------------

uint32_t YAsset::getId() {
    return m_nId;
}

// ---------------------------------------------------------------------

const std::vector<YAsset *> &YAsset::getChilds() {
    return m_vChilds;
}

// ---------------------------------------------------------------------

bool YAsset::hasChilds() {
    return m_vChilds.size() > 0;
}

// ---------------------------------------------------------------------

void YAsset::setParent(YAsset *pParent) {
    m_pParent = pParent;
}

// ---------------------------------------------------------------------

void YAsset::addChild(YAsset *pChild) {
    m_vChilds.push_back(pChild);
}

// ---------------------------------------------------------------------

void YAsset::setId(uint32_t nId) {
    m_nId = nId;
}

// ---------------------------------------------------------------------
// YAssetsService

YAssetsService::YAssetsService() {
    m_nLastId = 0;
    m_nUserVersion = 0;
    TAG = "YAssetsService";
}

// ---------------------------------------------------------------------

void YAssetsService::setUserVersionOfTree(uint32_t nUserVersion) {
    m_nUserVersion = nUserVersion;
}

// ---------------------------------------------------------------------

uint32_t YAssetsService::getUserVersionOfTree() {
    return m_nUserVersion;
}

// ---------------------------------------------------------------------

YAssetsService::~YAssetsService() {
    clearNodes();
    std::map<uint16_t, IFabricAsset*>::iterator it;
    for (it = m_mapFabricTreeNode.begin(); it != m_mapFabricTreeNode.end(); ++it) {
        delete it->second;
    }
    m_mapFabricTreeNode.clear();

}

// ---------------------------------------------------------------------

bool YAssetsService::readTreeFromFile(const std::string &sFilename, std::string &sError) {
    if (!WsjcppCore::fileExists(sFilename)) {
        sError = "File not exists";
        return false;
    }

    clearNodes();
    std::ifstream f;
    f.open(sFilename.c_str(), std::ios::in | std::ios::binary);

    char sExpectedFileHeader[20];
    f.read(sExpectedFileHeader, 20);
    if (!f) {
        sError = "readTreeFromFile. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }

    if (std::string(sExpectedFileHeader,20) != "WSJCPP-OBJ-TREE-FILE") {
        sError = "readTreeFromFile. Expected first 20 bytes of file like this WSJCPP-OBJ-TREE-FILE";
        return false;
    }

    // just a user version for usebillity
    if (!this->readUInt32(f, m_nUserVersion, sError)) {
        return false;
    }

    uint32_t nTreeSize = 0;
    if (!this->readUInt32(f, nTreeSize, sError)) {
        return false;
    }

    if (!this->readUInt32(f, m_nLastId, sError)) {
        return false;
    }

    std::map<uint32_t, YAsset *> mapTempIdToNode;
    for (int i = 0; i < nTreeSize; i++) {
        uint16_t nNodeType = 0;
        if (!this->readUInt16(f, nNodeType, sError)) {
            return false;
        }

        if (m_mapFabricTreeNode.find(nNodeType) == m_mapFabricTreeNode.end()) {
            sError = "On read file could not found node type: " + std::to_string(nNodeType);
            return false;
        }

        // read parent id
        uint32_t nParentId = 0;
        if (!this->readUInt32(f, nParentId, sError)) {
            return false;
        }
        YAsset *pParentNode = nullptr;

        if (mapTempIdToNode.find(nParentId) != mapTempIdToNode.end()) {
            pParentNode = mapTempIdToNode[nParentId];
        }

        // read node id
        uint32_t nNodeId = 0;
        if (!this->readUInt32(f, nNodeId, sError)) {
            return false;
        }
        

        if (nNodeId > m_nLastId) {
            sError = "Node id '" + std::to_string(nNodeId) + "' could not more then last id " + std::to_string(m_nLastId);
            return false;
        }

        YAsset *pNode = m_mapFabricTreeNode[nNodeType]->create();
        pNode->setId(nNodeId);
        pNode->setParent(pParentNode);
        if (pParentNode != nullptr) {
            pParentNode->addChild(pNode);
        }
        if (!pNode->readDataPartFromFile(f, sError)) {
            return false;
        }
        mapTempIdToNode[nNodeId] = pNode;
        m_vNodes.push_back(pNode);
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

bool YAssetsService::writeTreeToFile(const std::string &sFilename, std::string &sError) {
    std::ofstream f(sFilename.c_str(), std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to write " << sFilename << std::endl;
        return false;
    }
    static const std::string sFileHeader = "WSJCPP-OBJ-TREE-FILE";
    f.write(sFileHeader.c_str(), sFileHeader.length());

    // just a user version for usebillity
    this->writeUInt32(f, m_nUserVersion);
    
    int nTreeSize = m_vNodes.size();
    this->writeUInt32(f, nTreeSize);
    this->writeUInt32(f, m_nLastId);

    unsigned char arrShort[4];

    for (int i = 0; i < nTreeSize; i++) {
        YAsset *pNode = m_vNodes[i];

        // write type
        this->writeUInt16(f, pNode->getType());

        // write parent id
        if (pNode->getParent() != nullptr) {
            this->writeUInt32(f, pNode->getParent()->getId());
        } else {
            this->writeUInt32(f, 0);
        }
        
        // write id
        this->writeUInt32(f, pNode->getId());

        if (!pNode->writeDataPartToFile(f, sError)) {
            return false;
        }
    }
    f.close();
    return true;
}

// ---------------------------------------------------------------------

void YAssetsService::clearNodes() {
    int nSize = m_vNodes.size();
    for (int i = nSize - 1; i >=0; i--) {
        delete m_vNodes[i];
    }
    m_vNodes.clear();
}

// ---------------------------------------------------------------------

void YAssetsService::addNode(YAsset *pParent, YAsset *pChild) {
    if (pChild == nullptr) {
        WsjcppLog::throw_err(TAG, "::addNode - Child could not be null");
    }
    // check type of node
    if (!this->isSupportType(pChild->getType())) {
        WsjcppLog::throw_err(TAG, "::addNode - Not supported type");
    }

    std::vector<YAsset *>::iterator it;

    // check that parent node already exists in a list
    if (pParent != nullptr && !this->hasNode(pParent)) {
        WsjcppLog::throw_err(TAG, "::addNode - Not found parent node in the tree");
    }

    // check child node - exclude circles
    if (this->hasNode(pChild)) {
        WsjcppLog::throw_err(TAG, "::addNode - Child already added in the tree");
    }

    pChild->setParent(pParent);
    if (pParent != nullptr) {
        pParent->addChild(pChild);
    }
    m_nLastId++;
    pChild->setId(m_nLastId);
    m_vNodes.push_back(pChild);
}

// ---------------------------------------------------------------------

bool YAssetsService::isSupportType(uint16_t nType) {
    std::map<uint16_t, IFabricAsset*>::iterator it;
    it = m_mapFabricTreeNode.find(nType);
    return it != m_mapFabricTreeNode.end();
}

// ---------------------------------------------------------------------

bool YAssetsService::hasNode(YAsset *pNode) {
    std::vector<YAsset *>::iterator it;
    it = std::find(m_vNodes.begin(), m_vNodes.end(), pNode);
    return it != m_vNodes.end();
}

// ---------------------------------------------------------------------

const std::vector<YAsset *> &YAssetsService::getAllNodes() {
    return m_vNodes;
}

// ---------------------------------------------------------------------

int YAssetsService::getRoots(std::vector<YAsset *> &vRoots) {
    int nRet = 0;
    std::vector<YAsset *>::iterator it = m_vNodes.begin();
    for (it = m_vNodes.begin(); it != m_vNodes.end(); ++it) {
        if ((*it)->getParent() == nullptr) {
            vRoots.push_back(*it);
            nRet++;
        }
    }
    return nRet;
}

// ---------------------------------------------------------------------

std::string YAssetsService::toString() { // for printing
    std::string sIntent = "";
    return "Root (ver: " + std::to_string(m_nUserVersion) + ")\n" + toStringRecoursiveChilds(nullptr, sIntent);
}

// ---------------------------------------------------------------------

void YAssetsService::writeUInt32(std::ofstream &f, uint32_t nVal) {
    const char *pBuffer = reinterpret_cast<const char *>(&nVal);
    f.write(pBuffer, 4);
}

// ---------------------------------------------------------------------

bool YAssetsService::readUInt32(std::ifstream &f, uint32_t &nVal, std::string &sError) {
    // not for multithreading
    static char arrInteger[4];
    f.read(arrInteger, 4);
    if (!f) {
        sError = "Could not read. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nVal = *reinterpret_cast<uint32_t*>(arrInteger);
    return true;
}

// ---------------------------------------------------------------------

void YAssetsService::writeUInt16(std::ofstream &f, uint16_t nVal) {
    const char *pBuffer = reinterpret_cast<const char *>(&nVal);
    f.write(pBuffer, 2);
}

// ---------------------------------------------------------------------

bool YAssetsService::readUInt16(std::ifstream &f, uint16_t &nVal, std::string &sError) {
    // not for multithreading
    static char arrShort[2];
    f.read(arrShort, 2);
    if (!f) {
        sError = "Could not read. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nVal = *reinterpret_cast<uint16_t*>(arrShort);
    return true;
}

// ---------------------------------------------------------------------

std::string YAssetsService::toStringRecoursiveChilds(YAsset *pNode, const std::string &sIntent) {
    std::string sRet;
    std::vector<YAsset *> vChilds;
    if (pNode == nullptr) {
        getRoots(vChilds);
    } else {
        vChilds = pNode->getChilds();
    }
    int nLen = vChilds.size();
    for (int i = 0; i < nLen; i++) {
        bool bLatestChild = (i == nLen-1);
        YAsset *pNode_ = vChilds[i];
        sRet += sIntent;
        sRet += bLatestChild ? "└─ " : "├─ ";
        std::string sIntentAppend = bLatestChild ? "   " : "│  ";
        sRet += pNode_->toString(sIntent + sIntentAppend) + "\n";
        if (pNode_->hasChilds()) {
            sRet += toStringRecoursiveChilds(pNode_, sIntent + sIntentAppend);
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------
//WsjcppObjTreeChainDeclare

WsjcppObjTreeChainDeclare::WsjcppObjTreeChainDeclare(YAssetsService *pTree) {
    TAG = "WsjcppObjTreeChainDeclare";
    m_pTree = pTree;
    m_pCurrentNode = nullptr;
}

// ---------------------------------------------------------------------

WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::addNode(YAsset *pNode) {
    m_pTree->addNode(m_pCurrentNode, pNode);
    m_pCurrentNode = pNode;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::up() {
    if (m_pCurrentNode != nullptr) {
        m_pCurrentNode = m_pCurrentNode->getParent();
    }
    return *this;
}

// ---------------------------------------------------------------------

WsjcppObjTreeChainDeclare &WsjcppObjTreeChainDeclare::switchTo(YAsset *pNode) {
    if (!m_pTree->hasNode(pNode)) {
        WsjcppLog::throw_err(TAG, "::switchTo Not existed node in tree");
    }
    m_pCurrentNode = pNode;
    return *this;
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeString

WsjcppObjTreeNodeString::WsjcppObjTreeNodeString(YAssetsService *pTree, const std::string &sValue) 
: YAsset(pTree, WsjcppObjTreeNodeString::staticType()) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeString::getValue() {
    return m_sValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeString::setValue(const std::string &sValue) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeString::writeDataPartToFile(std::ofstream &f, std::string &sError) {
    uint32_t nStringLen = m_sValue.size();
    const char *pData = reinterpret_cast<const char *>(&nStringLen);
    f.write(pData, 4); // Write size of string
    f.write(m_sValue.c_str(), nStringLen);
    return true;
}; 

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeString::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    uint32_t nStringLen = 0;
    char arrInteger[4];
    f.read(arrInteger, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeString. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    nStringLen = *reinterpret_cast<uint32_t*>(arrInteger);
    char *pStr = new char[nStringLen];

    f.read(pStr, nStringLen);
    if (!f) {
        delete[] pStr;
        sError = "WsjcppObjTreeNodeString. Could not read string data. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    m_sValue = std::string(pStr, nStringLen);
    delete[] pStr;
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeString::toString(const std::string &sIntent) {
    return "string: " + m_sValue;
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeInteger

WsjcppObjTreeNodeInteger::WsjcppObjTreeNodeInteger(YAssetsService *pTree, const int32_t &nValue)
: YAsset(pTree, WsjcppObjTreeNodeInteger::staticType()) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

int32_t WsjcppObjTreeNodeInteger::getValue() {
    return m_nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeInteger::setValue(int32_t nValue) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeInteger::writeDataPartToFile(std::ofstream &f, std::string &sError) {
    static_assert(sizeof(uint32_t) == 4, "Expected sizeof(uint32_t) == 4");
    const char *pData = reinterpret_cast<const char *>(&m_nValue);
    f.write(pData, 4);
    return true;
}; 

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeInteger::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    // value 
    char arrBytes[4];
    f.read(arrBytes, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeInteger. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    static_assert(sizeof(uint32_t) == 4, "Expected sizeof(uint32_t) == 4");
    m_nValue = *reinterpret_cast<uint32_t*>(arrBytes);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeInteger::toString(const std::string &sIntent) {
    return "int: " + std::to_string(m_nValue);
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeFloat

WsjcppObjTreeNodeFloat::WsjcppObjTreeNodeFloat(YAssetsService *pTree, const float &nValue)
: YAsset(pTree, WsjcppObjTreeNodeFloat::staticType()) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

float WsjcppObjTreeNodeFloat::getValue() {
    return m_nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeFloat::setValue(float nValue) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeFloat::writeDataPartToFile(std::ofstream &f, std::string &sError) {
    static_assert(sizeof(float) == 4, "Expected sizeof(float) == 4");
    const char *pData = reinterpret_cast<const char *>(&m_nValue);
    f.write(pData, 4);
    return true;
}; 

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeFloat::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    static_assert(sizeof(float) == 4, "Expected sizeof(float) == 4");
    // value 
    char arrBytes[4];
    f.read(arrBytes, 4);
    if (!f) {
        sError = "WsjcppObjTreeNodeFloat. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    m_nValue = *reinterpret_cast<float*>(arrBytes);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeFloat::toString(const std::string &sIntent) {
    return "float: " + std::to_string(m_nValue);
}

// ---------------------------------------------------------------------
// WsjcppObjTreeNodeDouble

WsjcppObjTreeNodeDouble::WsjcppObjTreeNodeDouble(YAssetsService *pTree, const double &nValue)
: YAsset(pTree, WsjcppObjTreeNodeDouble::staticType()) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

float WsjcppObjTreeNodeDouble::getValue() {
    return m_nValue;
}

// ---------------------------------------------------------------------

void WsjcppObjTreeNodeDouble::setValue(float nValue) {
    m_nValue = nValue;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeDouble::writeDataPartToFile(std::ofstream &f, std::string &sError) {
    static_assert(sizeof(double) == 8, "Expected sizeof(double) == 8");
    const char *pData = reinterpret_cast<const char *>(&m_nValue);
    f.write(pData, 8);
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppObjTreeNodeDouble::readDataPartFromFile(std::ifstream &f, std::string &sError) {
    static_assert(sizeof(double) == 8, "Expected sizeof(double) == 8");
    // value
    char arrBytes[8];
    f.read(arrBytes, 8);
    if (!f) {
        sError = "WsjcppObjTreeNodeDouble. Could not read string len. File broken. Can read " + std::to_string(f.gcount());
        return false;
    }
    m_nValue = *reinterpret_cast<double*>(arrBytes);
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppObjTreeNodeDouble::toString(const std::string &sIntent) {
    return "double: " + std::to_string(m_nValue);
}
*/