#pragma once
#include <vector>
#include "coordxy.h"
#include "json.hpp"
#include <vector>
#include "game_building.h"

class GameState {

    public:
        GameState(int windowWidth, int windowHeight);
        void init();
        void updateElapsedTime();
        void addBuilding(GameBuilding *);
        long getElapsedTime() const;
        const CoordXY &getCoordLeftTop() const;
        void incrementCoordLeftTopX(int nX);
        void incrementCoordLeftTopY(int nY);
        
        const int windowWidth() const;
        const int windowHeight() const;

        void setMouseCaptured(bool bMouseCaptured);
        bool isMouseCaptured() const;

    private:
        bool m_bMouseCaptured;
        long m_nElapsedTime;
        long m_nStartTime;
        CoordXY m_coordLeftTop;
        int m_nWindowWidth;
        int m_nWindowHeight;
        std::vector<GameBuilding *> m_vBuildings;
};
