#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "game_rocket_state.h"

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
        GameRocketState *popRocket();

    private:
        CoordXY m_p0;
        bool m_bHasRocket;
        MoveObjectDirection m_nDirection;
        std::vector<GameRocketState *> m_vRockets;
};
