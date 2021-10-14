#pragma once

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <wsjcpp_core.h>
#include <ylog.h>

// ---------------------------------------------------------------------
// base employ class

class YServiceBase {
public:
    YServiceBase(
        const std::string &sName,
        const std::vector<std::string> &vLoadAfter
    );
    virtual ~YServiceBase();
    virtual bool init() = 0;
    virtual bool deinit() = 0;
    const std::vector<std::string> &loadAfter();

private:
    std::string TAG;
    std::string m_sName;
    std::vector<std::string> m_vLoadAfter;
};

// ---------------------------------------------------------------------
// public employees

extern std::map<std::string, YServiceBase*> *g_pYServices;
extern std::vector<std::string> *g_pYServicesInited;

class YServices {
    public:
        static void initGlobalVariables();
        static void deinitGlobalVariables();
        static void addService(const std::string &sName, YServiceBase* pService);
        static bool init(const std::vector<std::string> &vLoadAfter);
        static bool deinit();
        static void recoursiveTestDependencies(std::vector<std::string> v);
};

// ---------------------------------------------------------------------
// RegistryServiceLocator
#define REGISTRY_YSERVICE( classname ) \
    static classname * pRegistryYService ## classname = new classname();

// ---------------------------------------------------------------------
// findYService

template <class T> T* findYService() {
    YServices::initGlobalVariables();
    std::string TAG = "findYService";
    std::string sServiceName = T::name();
    YServiceBase *pService = NULL;
    if (g_pYServices->count(sServiceName)) {
        pService = g_pYServices->at(sServiceName);
    }
    if (pService == NULL) {
        YLog::throw_err(TAG, "Not found employ " + sServiceName);
    }
    T *pTEmploy = dynamic_cast<T*>(pService);
    if (pTEmploy == NULL) {
        YLog::throw_err(TAG, "Employ could not cast to T [" + sServiceName + "]");
    }
    return pTEmploy;
}

// ---------------------------------------------------------------------
// RuntimeGlobalCacheYService

class RuntimeGlobalCacheYService : public YServiceBase {
    public:
        RuntimeGlobalCacheYService();
        static std::string name() { return "RuntimeGlobalCacheYService"; }
        virtual bool init() override;
        virtual bool deinit() override;
        void set(const std::string &sName, const std::string &sValue);
        bool has(const std::string &sName);
        std::string get(const std::string &sName);

    private:
        std::string TAG;
        std::map<std::string, std::string> m_sStringMap;
};

