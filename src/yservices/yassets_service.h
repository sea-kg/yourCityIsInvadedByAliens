#pragma once

#include <yservices.h>
#include "window_yservice.h"
#include <string>
#include <map>
#include <fstream>
#include <ylog.h>
#include <ycore.h>
#include <yjson.h>
#include <memory>

class YAssetsService;

class YAsset {
    friend class YAssetsService;
    public:
        YAsset(YAssetsService *pAssetsService);
        virtual void setOrderZ(int nOrder) = 0;

    protected:
        YNeedUpdate m_needUpdate;
        YAssetsService *m_pAssetsService;
};

class YAssetFactory;

class YAssetFactoryType {
    friend class YAssetsService;
    public:
        YAssetFactoryType(YAssetsService *pAssetsService);
        virtual const std::wstring &getFactoryTypeId() const = 0;
        virtual YAssetFactory *createFactory(
            const std::wstring &sAssetFactoryPath,
            const std::wstring &sFactoryId,
            const YJsonObject &jsonFactoryConfig
        ) = 0;

    protected:
        std::wstring TAG;
        YAssetsService *m_pAssetsService;
};

class IYAssetFactoryFactoryType {
    public:
        virtual YAssetFactoryType *create(YAssetsService *pService) = 0;
};

class YAssetFactory {
    // friend class YAssetsService;
    public:
        YAssetFactory(YAssetsService *pAssetsService, YAssetFactoryType *pFactoryType);
        YAssetFactoryType *getFactoryType();
        virtual std::unique_ptr<YAsset> createAsset() = 0;

    protected:
        std::wstring TAG;
        YAssetsService *m_pAssetsService;
        YAssetFactoryType *m_pFactoryType;

        SDL_Texture *loadTexture(const std::wstring &sImagePath);
};

extern std::vector<IYAssetFactoryFactoryType*> *g_pYAssetFactoryFactoryType;

class YAssetsService : public YServiceBase {
    friend class YAsset;

    public:
        YAssetsService();
        static std::wstring name() { return L"YAssetsService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        RenderWindow *getRenderWindow();
        void registerFactoryType(YAssetFactoryType *);
        bool hasFactoryType(const std::wstring &sFactoryTypeId);
        bool loadAssetFactory(const std::wstring &sPath, std::wstring &sAssetFactoryType, std::wstring &sRetError);
        bool loadAllAssetFactory(const std::wstring &sPath, std::wstring &sRetError);
        std::vector<std::wstring> findFactoryIDsByFactoryType(const std::wstring &sPath);
        std::unique_ptr<YAsset> createAsset(const std::wstring &sAssetFactoryId);
        template<class T> T *createAsset(const std::wstring &sAssetFactoryId);

        static void registerFactoryFactoryType(IYAssetFactoryFactoryType* pService);

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
        std::unique_ptr<YAsset> asset{this->createAsset(sAssetFactoryId)}; \
        classname *pAssetRet = dynamic_cast<classname *>(asset.get()); \
		asset.release(); \
        if (pAssetRet == nullptr) { \
            YLog::throw_err(TAG, L"Could not cast for " + sAssetFactoryId + L" to "); \
        } \
        return pAssetRet; \
    }

// define for register assets-factories-types
#define REGISTRY_YASSET_FACTORY_TYPE( classname ) \
    class RegistryYAssetFactoryType ## classname : public IYAssetFactoryFactoryType { \
        public: \
            RegistryYAssetFactoryType ## classname () { \
                YAssetsService::registerFactoryFactoryType(this); \
            } \
            virtual YAssetFactoryType *create(YAssetsService *pService) override { \
                return new classname(pService); \
            } \
            classname *p; \
    }; \
    static RegistryYAssetFactoryType ## classname * pRegistryYAssetFactoryType ## classname \
        = new RegistryYAssetFactoryType ## classname();
