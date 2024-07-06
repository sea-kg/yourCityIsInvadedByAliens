
#include "map_yservice.h"
#include <iostream>
// #include <algorithm>
#include <ycore.h>


/*
// ---------------------------------------------------------------------
// MapRect

MapRect::MapRect(int x, int y, int w, int h) {
    m_nX0 = x;
    m_nY0 = y;
    m_nX1 = x + w;
    m_nY1 = y + h;
    m_nWidth = w;
    m_nHeight = h;
};

bool MapRect::hasPoint(int x, int y) const {
    return
        (x >= m_nX0)
        && (x <= m_nX1)
        && (y >= m_nY0)
        && (y <= m_nY1)
    ;
}

bool MapRect::hasIntersection(const MapRect& r) const {
    return
        (r.getX0() >= m_nX0)
        && (x <= m_nX1)
        && (y >= m_nY0)
        && (y <= m_nY1)
    ;
}

int MapRect::getX() const {
    return m_nX0;
}

int MapRect::getY() const {
    return m_nY0;
}

int MapRect::getWidth() const {
    return m_nWidth;
}

int MapRect::getHeight() const {
    return m_nHeight;
}
*/

// ---------------------------------------------------------------------
// MapYService

REGISTRY_YSERVICE(MapYService)

MapYService::MapYService()
    : YServiceBase(MapYService::name(), {}) {

    TAG = MapYService::name();
}

bool MapYService::init() {
    return true;
}

bool MapYService::deinit() {
    // checking settings
    YLog::info(TAG, L"deinit");
    return true;
}

void MapYService::addRoad(const YRect &road) {
    m_vRoads.push_back(road);
}

const std::vector<YRect> &MapYService::getRoads() {
    return m_vRoads;
}

void MapYService::addBuilding(const YRect &building) {
    m_vBuildings.push_back(building);
    YLog::info(TAG, std::to_wstring(m_vBuildings.size()));
}

bool MapYService::canDriveToPoint(const YPos &p) const {
    // TODO need optimization
    // YLog::info(TAG, std::to_wstring(m_vRoads.size()));
    for (int i = 0; i < m_vRoads.size(); i++) {
        if (m_vRoads[i].hasPos(p)) {
            return true;
        }
    }
    return false;
}

bool MapYService::isFreeRect(const YRect &r) const {
    for (int i = 0; i < m_vRoads.size(); i++) {
        if (m_vRoads[i].hasIntersection(r)) {
            return false;
        }
    }
    for (int i = 0; i < m_vBuildings.size(); i++) {
        if (m_vBuildings[i].hasIntersection(r)) {
            return false;
        }
    }
    return true;
}

void MapYService::addAlienBerry(GameAlienBerryState *pBerryState) {
    m_vAlienBerries.push_back(pBerryState);
}

const std::vector<GameAlienBerryState *> &MapYService::getAlienBerries() {
    return m_vAlienBerries;
}

// int MapYService::findAlienBerryIndex(int x, int y) {
//     for (int i = 0; i < m_vAlienBerries.size(); i++) {
//         if (m_vAlienBerries[i].hasPoint(x,y)) {
//             return i;
//         }
//     }
//     return -1;
// }

void MapYService::setMapSize(int nMapWidth, int nMapHeight) {
    m_nMapWidth = nMapWidth;
    m_nMapHeight = nMapHeight;
}

int MapYService::getMapWidth() const {
    return m_nMapWidth;
}

int MapYService::getMapHeight() const {
    return m_nMapHeight;
}
