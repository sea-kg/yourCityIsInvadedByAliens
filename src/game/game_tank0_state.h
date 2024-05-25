#pragma once
#include <vector>
#include <string>
#include "move_object_direction.h"
#include "game_rocket_state.h"

class GameTank0State {
    public:
        GameTank0State(const YPos &p0);
        MoveObjectDirection getDirection();
        const YPos &getPosition();
        void turnLeft();
        void turnRight();
        YPos calculateMoveForward();
        void moveForward();
        bool hasRocket();
        void rechargeRocket();
        void shotRocket();
        GameRocketState *popRocket();

    private:
        std::wstring TAG;
        YPos m_p0;
        int m_nMoveStep;
        bool m_bHasRocket;
        MoveObjectDirection m_nDirection;
        std::vector<GameRocketState *> m_vRockets;
        std::vector<YPos> m_vMoveVectors;
};
