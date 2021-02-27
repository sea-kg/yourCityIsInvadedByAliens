#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "json.hpp"

class GameRocketState {
    public:
        GameRocketState(const CoordXY &p0, MoveObjectDirection direction);
        MoveObjectDirection getDirection();
        const CoordXY &getPosition();
        void move();
        bool hasDestroyed();
        void destroy();

    private:
        CoordXY m_p0;
        bool m_bDestroyed;
        MoveObjectDirection m_nDirection;
};
