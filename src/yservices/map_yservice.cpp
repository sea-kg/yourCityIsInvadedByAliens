
#include "map_yservice.h"
#include <iostream>
// #include <algorithm>
#include <ycore.h>


// ---------------------------------------------------------------------
// MapRoad

MapRoad::MapRoad(int x, int y, int w, int h) {
    m_nX = x;
    m_nY = y;
    m_nWidth = w;
    m_nHeight = h;
};

bool MapRoad::hasPoint(int x, int y) {
    return
        (x >= m_nX)
        && (x <= (m_nX + m_nWidth))
        && (y >= m_nY)
        && (y <= (m_nY + m_nHeight))
    ;
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

void MapYService::addRoad(const MapRoad &road) {
    m_vRoads.push_back(road);
}

bool MapYService::canDriveToPoint(int x, int y) {
    // TODO need optimization
    YLog::info(TAG, std::to_wstring(m_vRoads.size()));
    for (int i = 0; i < m_vRoads.size(); i++) {
        if (m_vRoads[i].hasPoint(x, y)) {
            return true;
        }
    }
    return false;
}