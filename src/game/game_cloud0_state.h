#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "game_rocket_state.h"

class GameCloud0State {
    public:
        GameCloud0State(const CoordXY &p0);
        const CoordXY &getPosition();
        void move();

    private:
        void randomDiff();

        CoordXY m_p0;
        CoordXY m_pDiff;
        int m_nMoves;
        int m_nMaxMoves;
        
};
