#pragma once

#include "yservices.h"

// ---------------------------------------------------------------------
// MapRect

class MapRect {
    public:
        MapRect(int x, int y, int w, int h);
        bool hasPoint(int x, int y);
        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;
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

        void addRoad(const MapRect &road);
        const std::vector<MapRect> &getRoads();
        bool canDriveToPoint(int x, int y);
        void addAlienBerry(const MapRect &berry);
        const std::vector<MapRect> &getAlienBerries();
        int findAlienBerryIndex(int x, int y);
        void setMapSize(int nMapWidth, int nMapHeight);
        int getMapWidth() const;
        int getMapHeight() const;

    private:
        std::wstring TAG;
        
        std::vector<MapRect> m_vRoads;
        std::vector<MapRect> m_vAlienBerries;
        int m_nMapWidth;
        int m_nMapHeight;
};