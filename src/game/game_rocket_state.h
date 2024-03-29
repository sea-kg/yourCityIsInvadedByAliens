#pragma once
#include <vector>
#include "yrect.h"
#include "move_object_direction.h"

class GameRocketState {
    public:
        GameRocketState(const YPos &p0, MoveObjectDirection direction);
        MoveObjectDirection getDirection();
        const YPos &getPosition();
        void move();
        bool hasDestroyed();
        void destroy();
        void explode();
        bool isExploded();
        bool canBeRemoved();
        void removeLater();

    private:
        YPos m_p0;
        bool m_bDestroyed;
        bool m_bExploded;
        bool m_bCanBeRemoved;
        MoveObjectDirection m_nDirection;
};
