#pragma once

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <ylog.h>

// ---------------------------------------------------------------------
// base employ class

class YServiceBase {
public:
    YServiceBase(
        const std::wstring &sName,
        const std::vector<std::wstring> &vLoadAfter
    );
    virtual ~YServiceBase();
    virtual bool init() = 0;
    virtual bool deinit() = 0;
    const std::vector<std::wstring> &loadAfter();

private:
    std::wstring TAG;
    std::wstring m_sName;
    std::vector<std::wstring> m_vLoadAfter;
};

// ---------------------------------------------------------------------
// public employees

extern std::map<std::wstring, YServiceBase*> *g_pYServices;
extern std::vector<std::wstring> *g_pYServicesInited;

class YServices {
    public:
        static void initGlobalVariables();
        static void deinitGlobalVariables();
        static void addService(const std::wstring &sName, YServiceBase* pService);
        static bool init(const std::vector<std::wstring> &vLoadAfter);
        static bool deinit();
        static void recoursiveTestDependencies(std::vector<std::wstring> v);
};

// ---------------------------------------------------------------------
// RegistryServiceLocator
#define REGISTRY_YSERVICE( classname ) \
    static classname * pRegistryYService ## classname = new classname();

// ---------------------------------------------------------------------
// findYService

template <class T> T* findYService() {
    YServices::initGlobalVariables();
    std::wstring TAG = L"findYService";
    std::wstring sServiceName = T::name();
    YServiceBase *pService = NULL;
    if (g_pYServices->count(sServiceName)) {
        pService = g_pYServices->at(sServiceName);
    }
    if (pService == NULL) {
        YLog::throw_err(TAG, L"Not found employ " + sServiceName);
    }
    T *pTEmploy = dynamic_cast<T*>(pService);
    if (pTEmploy == NULL) {
        YLog::throw_err(TAG, L"Employ could not cast to T [" + sServiceName + L"]");
    }
    return pTEmploy;
}


