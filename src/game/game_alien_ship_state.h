#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "game_bioplast_state.h"
#include "game_rocket_state.h"

class GameAlienShipState {
    public:
        GameAlienShipState(const CoordXY &p0);
        const CoordXY &getPosition();
        void setShooting(bool bShooting);
        bool isShooting() const;
        void setMoveDirection(MoveObjectDirection direction);
        void move(
            long nElapsedTime,
            const CoordXY &minPointMap,
            const CoordXY &maxPointMap,
            int nLeftPad,
            int nRightPad,
            int nTopPad,
            int nBottomPad
        );
        void shot();
        GameBioplastState *popRocket();
        void rocketAttack(GameRocketState *pRocket);
        
    private:
        CoordXY m_p0;
        MoveObjectDirection m_moveDirection;
        long m_nMovePrevTime;
        long m_nSpeedMoving;
        bool m_bShooting;
        std::vector<GameBioplastState *> m_vBioplasts;
        int m_nHealthPoints;
};
