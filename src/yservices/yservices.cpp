#include "yservices.h"
#include <algorithm>

#include <wsjcpp_core.h>
#include <iostream>

// ---------------------------------------------------------------------

std::map<std::string, YServiceBase*> *g_pYServices = nullptr;
std::vector<std::string> *g_pYServicesInited = nullptr;
std::vector<std::string> *g_pWsjcppInitWith = nullptr;

// ---------------------------------------------------------------------

void YServices::initGlobalVariables() {
    if (g_pYServices == nullptr) {
        // YLog::info(std::string(), "Create employees map");
        g_pYServices = new std::map<std::string, YServiceBase*>();
    }
    if (g_pYServicesInited == nullptr) {
        // YLog::info(std::string(), "Create init employees vector");
        g_pYServicesInited = new std::vector<std::string>();
    }
    if (g_pWsjcppInitWith == nullptr) {
        // YLog::info(std::string(), "Create init employees vector");
        g_pWsjcppInitWith = new std::vector<std::string>();
    }    
}

// ---------------------------------------------------------------------

void YServices::deinitGlobalVariables() {
    const std::string TAG = "YServices::deinit";
    if (g_pYServices != nullptr) {
        std::map<std::string, YServiceBase*>::iterator it;
        for (it = g_pYServices->begin(); it != g_pYServices->end(); ++it) {
            std::string sEmployName = it->first;
            YLog::ok(TAG, sEmployName + " ... UNREGISTERED");
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

void YServices::addService(const std::string &sName, YServiceBase* pEmploy) {
    YServices::initGlobalVariables();
    if (g_pYServices->find(sName) != g_pYServices->end()) {
        YLog::throw_err("YServices::addService", "Already registered '" + sName + "'");
    } else {
        g_pYServices->insert(std::pair<std::string, YServiceBase*>(sName,pEmploy));
        // YLog::info(sName, "Registered");
    }
}

// ---------------------------------------------------------------------

bool YServices::init(const std::vector<std::string> &vStart) {
    YServices::initGlobalVariables();
    std::string TAG = "YServices::init";

    for (unsigned int i = 0; i < vStart.size(); i++) {
        g_pYServicesInited->push_back(vStart[i]);
        g_pWsjcppInitWith->push_back(vStart[i]);
        YLog::info(TAG, "with " + vStart[i]);
    }
    
    bool bRepeat = true;
    while (bRepeat) {
        bRepeat = false;
        std::map<std::string, YServiceBase*>::iterator it = g_pYServices->begin();
        for (; it!=g_pYServices->end(); ++it) {
            std::string sEmployName = it->first;
            YServiceBase *pEmploy = it->second;

            if (std::find(g_pYServicesInited->begin(), g_pYServicesInited->end(), sEmployName) != g_pYServicesInited->end()) {
                continue;
            }

            unsigned int nRequireLoaded = 0;
            for (unsigned int i = 0; i < pEmploy->loadAfter().size(); i++) {
                std::string sRequireEmploy = pEmploy->loadAfter()[i];
                if (std::find(g_pYServicesInited->begin(), g_pYServicesInited->end(), sRequireEmploy) != g_pYServicesInited->end()) {
                    nRequireLoaded++;
                }
            }
            if (pEmploy->loadAfter().size() == nRequireLoaded) {
                if (!pEmploy->init()) {
                    YLog::err(TAG, sEmployName + " ... INIT_FAIL");
                    return false;
                }
                g_pYServicesInited->push_back(sEmployName);
                bRepeat = true;
                YLog::ok(TAG, sEmployName + " ... INIT_OK");
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool YServices::deinit() {
    const std::string TAG = "YServices::deinit";
    if (g_pYServicesInited == nullptr
        || g_pWsjcppInitWith == nullptr 
        || g_pYServices == nullptr
    ) {
        YLog::err(TAG, "You must call YServices::init before deinit");
        return false;
    }

    int nInitedCount = g_pYServicesInited->size();
    for (int i = nInitedCount-1; i >= 0; i--) {
        std::string sEmployName = g_pYServicesInited->at(i);
        if (std::find(g_pWsjcppInitWith->begin(), g_pWsjcppInitWith->end(), sEmployName) != g_pWsjcppInitWith->end()) {
            YLog::info(TAG,  sEmployName + " ... SKIP_INIT_WITH");
            continue;
        } 

        std::map<std::string, YServiceBase*>::iterator it;
        it = g_pYServices->find(sEmployName);
        if (it == g_pYServices->end()) {
            YLog::err(TAG,  sEmployName + " ... DEINIT_NOT_FOUND");
            return false;
        }
        YServiceBase *pEmploy = it->second;
        if (pEmploy->deinit()) {
            YLog::ok(TAG, sEmployName + " ... DEINIT_OK");
        } else {
            YLog::err(TAG,  sEmployName + " ... DEINIT_FAIL");
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

void YServices::recoursiveTestDependencies(std::vector<std::string> v) {
    std::string sEmployName = v[v.size()-1];
    YServiceBase *pEmploy = nullptr;
    
    std::map<std::string, YServiceBase*>::iterator it;
    it = g_pYServices->find(sEmployName);
    if (it == g_pYServices->end()) {
        // YLog::throw_err("YServices::recoursiveTestDependencies", "Not found employ '" + sEmployName + "'");
        return;
    }
    pEmploy = g_pYServices->at(sEmployName);
    std::vector<std::string> vLoadAfter = pEmploy->loadAfter();
    for (int la = 0; la < vLoadAfter.size(); la++) {
        for (int i = 0; i < v.size(); i++) {
            if (v[i] == vLoadAfter[la]) {
                YLog::throw_err(
                    "YServices::recoursiveTestDependencies", 
                    "Cicle dependency: " + WsjcppCore::join(v, " -> ") + " -> " + vLoadAfter[la]
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

YServiceBase::YServiceBase(const std::string &sName, const std::vector<std::string> &vAfter) {
    TAG = sName;
    m_sName = sName;

    for (unsigned int i = 0; i < vAfter.size(); i++) {
        m_vLoadAfter.push_back(vAfter[i]);
    }
    YServices::addService(m_sName, this);
    std::vector<std::string> vInitialVector;
    vInitialVector.push_back(m_sName);
    YServices::recoursiveTestDependencies(vInitialVector);
}

// ---------------------------------------------------------------------

YServiceBase::~YServiceBase() {
    // nothing
}

// ---------------------------------------------------------------------

const std::vector<std::string> &YServiceBase::loadAfter() {
    return m_vLoadAfter;
}


