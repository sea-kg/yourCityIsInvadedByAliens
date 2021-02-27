#pragma once
#include <vector>
#include "coordxy.h"
#include "json.hpp"
#include <vector>
#include "game_building.h"
#include "move_object_direction.h"

class GameState {

    public:
        GameState(int windowWidth, int windowHeight);
        void init();
        void updateElapsedTime();
        void addBuilding(GameBuilding *);
        long getElapsedTime() const;
        const CoordXY &getCoordLeftTop() const;
        void setMovePlayerDirection(MoveObjectDirection direction);
        void movePlayer();
       
        const int windowWidth() const;
        const int windowHeight() const;

        void setMouseCaptured(bool bMouseCaptured);
        bool isMouseCaptured() const;

        void setMinPoint(const CoordXY &p);
        const CoordXY &getMinPoint();
        void setMaxPoint(const CoordXY &p);
        const CoordXY &getMaxPoint();

    private:
        MoveObjectDirection m_playerDirection;
        long m_nPlayerPrevTime;
        bool m_bMouseCaptured;
        long m_nElapsedTime;
        long m_nStartTime;
        CoordXY m_coordLeftTop;
        CoordXY m_maxPoint;
        CoordXY m_minPoint;
        int m_nWindowWidth;
        int m_nWindowHeight;
        std::vector<GameBuilding *> m_vBuildings;
};
