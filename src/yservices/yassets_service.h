#pragma once

#include <yservices.h>
#include "window_yservice.h"
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
        virtual void setOrderZ(int nOrder) = 0;

    protected:
        YAssetsService *m_pAssetsService;
};

class YAssetFactory;

class YAssetFactoryType {
    friend class YAssetsService;
    public:
        YAssetFactoryType(YAssetsService *pAssetsService);
        virtual std::wstring getFabricTypeId() = 0;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) = 0;

    protected:
        std::wstring TAG;
        YAssetsService *m_pAssetsService;
};

class YAssetFactory {
    // friend class YAssetsService;
    public:
        YAssetFactory(YAssetsService *pAssetsService, YAssetFactoryType *pFactoryType);
        virtual YAsset *createAsset() = 0;

    protected:
        std::wstring TAG;
        YAssetsService *m_pAssetsService;
        YAssetFactoryType *m_pFactoryType;
};

class YAssetsService : public YServiceBase {
    friend class YAsset;

    public:
        YAssetsService();
        static std::wstring name() { return L"YAssetsService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        RenderWindow *getRenderWindow();
        void registerFabricType(YAssetFactoryType *);
        bool hasFabricType(const std::wstring &sFactoryTypeId);
        bool loadAssetFactory(const std::wstring &sPath, std::wstring &sRetError);
        
        YAsset *createAsset(const std::wstring &sAssetFactoryId);
        template<class T> T *createAsset(const std::wstring &sAssetFactoryId);

    private:
        std::wstring TAG;
        std::map<std::wstring, YAssetFactoryType*> m_mapYAssetsFactoryTypes;
        std::map<std::wstring, YAssetFactory*> m_mapYAssetsFactories;
        RenderWindow *m_pRenderWindow;
};

// define for inline templates
#define YASSET_DECLARE_INLINE( classname ) \
    template<> inline \
    classname *YAssetsService::createAsset< classname >(const std::wstring &sAssetFactoryId) { \
        YAsset *pAsset = this->createAsset(sAssetFactoryId); \
        classname *pAssetRet = dynamic_cast<classname *>(pAsset); \
        if (pAssetRet == nullptr) { \
            YLog::throw_err(TAG, L"Could not cast for " + sAssetFactoryId + L" to "); \
        } \
        return pAssetRet; \
    }
