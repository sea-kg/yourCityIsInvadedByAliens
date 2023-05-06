
#include "map_yservice.h"
#include <iostream>
// #include <algorithm>
#include <ycore.h>


// ---------------------------------------------------------------------
// MapRect

MapRect::MapRect(int x, int y, int w, int h) {
    m_nX = x;
    m_nY = y;
    m_nWidth = w;
    m_nHeight = h;
};

bool MapRect::hasPoint(int x, int y) {
    return
        (x >= m_nX)
        && (x <= (m_nX + m_nWidth))
        && (y >= m_nY)
        && (y <= (m_nY + m_nHeight))
    ;
}

int MapRect::getX() const {
    return m_nX;
}

int MapRect::getY() const {
    return m_nY;
}

int MapRect::getWidth() const {
    return m_nWidth;
}

int MapRect::getHeight() const {
    return m_nHeight;
}


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

void MapYService::addRoad(const MapRect &road) {
    m_vRoads.push_back(road);
}

const std::vector<MapRect> &MapYService::getRoads() {
    return m_vRoads;
}

bool MapYService::canDriveToPoint(int x, int y) {
    // TODO need optimization
    // YLog::info(TAG, std::to_wstring(m_vRoads.size()));
    for (int i = 0; i < m_vRoads.size(); i++) {
        if (m_vRoads[i].hasPoint(x, y)) {
            return true;
        }
    }
    return false;
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
