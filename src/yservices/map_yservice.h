#pragma once

#include "yservices.h"

// ---------------------------------------------------------------------
// MapRoad

class MapRoad {
    public:
        MapRoad(int x, int y, int w, int h);
        bool hasPoint(int x, int y);
    private:
        int m_nX;
        int m_nY;
        int m_nWidth;
        int m_nHeight;
};

// ---------------------------------------------------------------------
// MapYService

class MapYService : public YServiceBase {
    public:
        MapYService();
        static std::wstring name() { return L"MapYService"; }
        virtual bool init() override;
        virtual bool deinit() override;

        void addRoad(const MapRoad &road);
        bool canDriveToPoint(int x, int y);
    private:
        std::wstring TAG;
        
        std::vector<MapRoad> m_vRoads;
};