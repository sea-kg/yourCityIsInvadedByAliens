#pragma once

#include <yservices.h>
#include <wsjcpp_core.h>
#include "render_window.h"
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
        virtual RenderObject *getRenderObject() = 0;

    protected:
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
        virtual YAsset *createAsset() = 0;

    protected:
        std::string TAG;
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
        void setRenderWindow(RenderWindow *pRenderWindow);
        RenderWindow *getRenderWindow();
        void registerFabricType(YAssetFactoryType *);
        bool hasFabricType(const std::string &sFactoryTypeId);
        bool loadAssetFactory(const std::string &sPath, std::string &sRetError);
        
        YAsset *createAsset(const std::string &sAssetFactoryId);
        template<class T> T *createAsset(const std::string &sAssetFactoryId);

    private:
        std::string TAG;
        std::map<std::string, YAssetFactoryType*> m_mapYAssetsFactoryTypes;
        std::map<std::string, YAssetFactory*> m_mapYAssetsFactories;
        RenderWindow *m_pRenderWindow;
};

// define for inline templates
#define YASSET_DECLARE_INLINE( classname ) \
    template<> inline \
    classname *YAssetsService::createAsset< classname >(const std::string &sAssetFactoryId) { \
        YAsset *pAsset = this->createAsset(sAssetFactoryId); \
        classname *pAssetRet = dynamic_cast<classname *>(pAsset); \
        if (pAssetRet == nullptr) { \
            YLog::throw_err(TAG, "Could not cast for " + sAssetFactoryId + " to "); \
        } \
        return pAssetRet; \
    }
