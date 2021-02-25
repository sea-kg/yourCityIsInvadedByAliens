#pragma once
#include <vector>
#include "coordxy.h"
#include "json.hpp"

class GameBuilding {
    public:
        GameBuilding(nlohmann::json &jsonData);
        const std::string &getName();
        const std::vector<CoordXY> &getPoints();

    private:
        std::string m_sName;
        std::vector<CoordXY> m_vPoints;
};

enum class MoveObjectDirection {
    UP,
    DOWN,
    UP_LEFT,
    DOWN_LEFT,
    UP_RIGHT,
    DOWN_RIGHT,
    LEFT,
    RIGHT
};

class GameTank0State {
    public:
        GameTank0State(const CoordXY &p0);
        MoveObjectDirection getDirection();
        const CoordXY &getPosition();
        void turnLeft();
        void turnRight();
        void move();
        bool hasRocket();
        void rechargeRocket();
        void shotRocket();

    private:
        CoordXY m_p0;
        bool m_bHasRocket;
        MoveObjectDirection m_nDirection;
};

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
