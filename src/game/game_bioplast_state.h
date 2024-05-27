#pragma once
#include <vector>
#include "ypos.h"

class GameBioplastState {
    public:
        GameBioplastState(const YPos &pStart, const YPos &pEnd);
        const YPos &getPosition();
        void move();
        bool hasDestroyed();
        void destroy();
        bool canBeRemoved();
        void removeLater();

    private:
        YPos m_pStart;
        YPos m_pEnd;
        YPos m_p0;
        bool m_bDestroyed;
        bool m_bCanBeRemoved;
        double m_nDistance;
        YPos m_step;
};
