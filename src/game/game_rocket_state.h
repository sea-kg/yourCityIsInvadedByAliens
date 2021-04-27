#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"

class GameRocketState {
    public:
        GameRocketState(const CoordXY &p0, MoveObjectDirection direction);
        MoveObjectDirection getDirection();
        const CoordXY &getPosition();
        void move();
        bool hasDestroyed();
        void destroy();
        void explode();
        bool isExploded();

    private:
        CoordXY m_p0;
        bool m_bDestroyed;
        bool m_bExploded;
        MoveObjectDirection m_nDirection;
};
