#pragma once
#include <vector>
#include "yrect.h"
#include "move_object_direction.h"

class GameAlienBerryState {
    public:
        GameAlienBerryState(const YPos &p0, int nWidth, int nHeight);
        const YPos &getPosition();
        // void move();
        bool hasDestroyed();
        void destroy();
        void explode();
        bool isExploded();

        bool hasPoint(int x, int y);
        int getWidth() const;
        int getHeight() const;

    private:
        YPos m_p0;
        int m_nWidth;
        int m_nHeight;
        bool m_bDestroyed;
        bool m_bExploded;
};
