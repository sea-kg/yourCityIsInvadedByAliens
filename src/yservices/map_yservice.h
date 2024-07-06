#pragma once

#include "yservices.h"
#include "game_alien_berry_state.h"

// ---------------------------------------------------------------------
// MapYService

class MapYService : public YServiceBase {
    public:
        MapYService();
        static std::wstring name() { return L"MapYService"; }
        virtual bool init() override;
        virtual bool deinit() override;

        void addRoad(const YRect &road);
        const std::vector<YRect> &getRoads();
        void addBuilding(const YRect &road);
        bool canDriveToPoint(const YPos &p) const;
        bool isFreeRect(const YRect &r0) const;
        void addAlienBerry(GameAlienBerryState *pBerryState);
        const std::vector<GameAlienBerryState *> &getAlienBerries();
        void setMapSize(int nMapWidth, int nMapHeight);
        int getMapWidth() const;
        int getMapHeight() const;

    private:
        std::wstring TAG;

        std::vector<YRect> m_vRoads;
        std::vector<YRect> m_vBuildings;
        std::vector<GameAlienBerryState *> m_vAlienBerries;
        int m_nMapWidth;
        int m_nMapHeight;
};