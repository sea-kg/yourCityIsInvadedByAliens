#pragma once
#include <vector>
#include "coordxy.h"

class GameBioplastState {
    public:
        GameBioplastState(const CoordXY &pStart, const CoordXY &pEnd);
        const CoordXY &getPosition();
        void move();
        bool hasDestroyed();
        void destroy();
        bool canBeRemoved();
        void removeLater();

    private:
        CoordXY m_pStart;
        CoordXY m_pEnd;
        CoordXY m_p0;
        bool m_bDestroyed;
        bool m_bCanBeRemoved;
        double m_nDistance;
        CoordXY m_step;
};
