#include "yservices.h"
#include <algorithm>
#include <iostream>
#include <ycore.h>

// ---------------------------------------------------------------------

std::map<std::wstring, YServiceBase*> *g_pYServices = nullptr;
std::vector<std::wstring> *g_pYServicesInited = nullptr;
std::vector<std::wstring> *g_pWsjcppInitWith = nullptr;

// ---------------------------------------------------------------------

void YServices::initGlobalVariables() {
    if (g_pYServices == nullptr) {
        // YLog::info(std::wstring(), L"Create employees map");
        g_pYServices = new std::map<std::wstring, YServiceBase*>();
    }
    if (g_pYServicesInited == nullptr) {
        // YLog::info(std::wstring(), L"Create init employees vector");
        g_pYServicesInited = new std::vector<std::wstring>();
    }
    if (g_pWsjcppInitWith == nullptr) {
        // YLog::info(std::wstring(), L"Create init employees vector");
        g_pWsjcppInitWith = new std::vector<std::wstring>();
    }    
}

// ---------------------------------------------------------------------

void YServices::deinitGlobalVariables() {
    const std::wstring TAG = L"YServices::deinit";
    if (g_pYServices != nullptr) {
        std::map<std::wstring, YServiceBase*>::iterator it;
        for (it = g_pYServices->begin(); it != g_pYServices->end(); ++it) {
            std::wstring sEmployName = it->first;
            YLog::ok(TAG, sEmployName + L" ... UNREGISTERED");
            delete it->second;
            it->second = nullptr;
        }
        g_pYServices->clear();
        delete g_pYServices;
        g_pYServices = nullptr;
    }

    if (g_pYServicesInited != nullptr) {
        g_pYServicesInited->clear();
        delete g_pYServicesInited;
        g_pYServicesInited = nullptr;
    }

    if (g_pWsjcppInitWith != nullptr) {
        g_pWsjcppInitWith->clear();
        delete g_pWsjcppInitWith;
        g_pWsjcppInitWith = nullptr;
    }
}

// ---------------------------------------------------------------------

void YServices::addService(const std::wstring &sName, YServiceBase* pEmploy) {
    YServices::initGlobalVariables();
    if (g_pYServices->find(sName) != g_pYServices->end()) {
        YLog::throw_err(L"YServices::addService", L"Already registered '" + sName + L"'");
    } else {
        g_pYServices->insert(std::pair<std::wstring, YServiceBase*>(sName,pEmploy));
        // YLog::info(sName, L"Registered");
    }
}

// ---------------------------------------------------------------------

bool YServices::init(const std::vector<std::wstring> &vStart) {
    YServices::initGlobalVariables();
    std::wstring TAG = L"YServices::init";

    for (unsigned int i = 0; i < vStart.size(); i++) {
        g_pYServicesInited->push_back(vStart[i]);
        g_pWsjcppInitWith->push_back(vStart[i]);
        YLog::info(TAG, L"with " + vStart[i]);
    }
    
    bool bRepeat = true;
    while (bRepeat) {
        bRepeat = false;
        std::map<std::wstring, YServiceBase*>::iterator it = g_pYServices->begin();
        for (; it!=g_pYServices->end(); ++it) {
            std::wstring sEmployName = it->first;
            YServiceBase *pEmploy = it->second;

            if (std::find(g_pYServicesInited->begin(), g_pYServicesInited->end(), sEmployName) != g_pYServicesInited->end()) {
                continue;
            }

            unsigned int nRequireLoaded = 0;
            for (unsigned int i = 0; i < pEmploy->loadAfter().size(); i++) {
                std::wstring sRequireEmploy = pEmploy->loadAfter()[i];
                if (std::find(g_pYServicesInited->begin(), g_pYServicesInited->end(), sRequireEmploy) != g_pYServicesInited->end()) {
                    nRequireLoaded++;
                }
            }
            if (pEmploy->loadAfter().size() == nRequireLoaded) {
                if (!pEmploy->init()) {
                    YLog::err(TAG, sEmployName + L" ... INIT_FAIL");
                    return false;
                }
                g_pYServicesInited->push_back(sEmployName);
                bRepeat = true;
                YLog::ok(TAG, sEmployName + L" ... INIT_OK");
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool YServices::deinit() {
    const std::wstring TAG = L"YServices::deinit";
    if (g_pYServicesInited == nullptr
        || g_pWsjcppInitWith == nullptr 
        || g_pYServices == nullptr
    ) {
        YLog::err(TAG, L"You must call YServices::init before deinit");
        return false;
    }

    int nInitedCount = g_pYServicesInited->size();
    for (int i = nInitedCount-1; i >= 0; i--) {
        std::wstring sEmployName = g_pYServicesInited->at(i);
        if (std::find(g_pWsjcppInitWith->begin(), g_pWsjcppInitWith->end(), sEmployName) != g_pWsjcppInitWith->end()) {
            YLog::info(TAG,  sEmployName + L" ... SKIP_INIT_WITH");
            continue;
        } 

        std::map<std::wstring, YServiceBase*>::iterator it;
        it = g_pYServices->find(sEmployName);
        if (it == g_pYServices->end()) {
            YLog::err(TAG,  sEmployName + L" ... DEINIT_NOT_FOUND");
            return false;
        }
        YServiceBase *pEmploy = it->second;
        if (pEmploy->deinit()) {
            YLog::ok(TAG, sEmployName + L" ... DEINIT_OK");
        } else {
            YLog::err(TAG,  sEmployName + L" ... DEINIT_FAIL");
            return false;
        }
    };

    g_pYServicesInited->clear();
    delete g_pYServicesInited;
    g_pYServicesInited = nullptr;

    g_pWsjcppInitWith->clear();
    delete g_pWsjcppInitWith;
    g_pWsjcppInitWith = nullptr;
    return true;
}

// ---------------------------------------------------------------------

void YServices::recoursiveTestDependencies(std::vector<std::wstring> v) {
    std::wstring sEmployName = v[v.size()-1];
    YServiceBase *pEmploy = nullptr;
    
    std::map<std::wstring, YServiceBase*>::iterator it;
    it = g_pYServices->find(sEmployName);
    if (it == g_pYServices->end()) {
        // YLog::throw_err("YServices::recoursiveTestDependencies", "Not found employ '" + sEmployName + "'");
        return;
    }
    pEmploy = g_pYServices->at(sEmployName);
    std::vector<std::wstring> vLoadAfter = pEmploy->loadAfter();
    for (int la = 0; la < vLoadAfter.size(); la++) {
        for (int i = 0; i < v.size(); i++) {
            if (v[i] == vLoadAfter[la]) {
                YLog::throw_err(
                    L"YServices::recoursiveTestDependencies", 
                    L"Cicle dependency: " + YCore::join(v, L" -> ") + L" -> " + vLoadAfter[la]
                );
                return;
            }
        }
        v.push_back(vLoadAfter[la]);
        recoursiveTestDependencies(v);
        v.pop_back();
    }
}

// ---------------------------------------------------------------------
// YServiceBase

YServiceBase::YServiceBase(const std::wstring &sName, const std::vector<std::wstring> &vAfter) {
    TAG = sName;
    m_sName = sName;

    for (unsigned int i = 0; i < vAfter.size(); i++) {
        m_vLoadAfter.push_back(vAfter[i]);
    }
    YServices::addService(m_sName, this);
    std::vector<std::wstring> vInitialVector;
    vInitialVector.push_back(m_sName);
    YServices::recoursiveTestDependencies(vInitialVector);
}

// ---------------------------------------------------------------------

YServiceBase::~YServiceBase() {
    // nothing
}

// ---------------------------------------------------------------------

const std::vector<std::wstring> &YServiceBase::loadAfter() {
    return m_vLoadAfter;
}


