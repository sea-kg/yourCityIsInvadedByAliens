#pragma once

#include "ypos.h"

class YRect {
    public:
        YRect();
        YRect(int nXMin, int nXMax, int nYMin, int nYMax);
        const int &getMinX() const;
        const int &getMaxX() const;
        const int &getMinY() const;
        const int &getMaxY() const;

        bool hasIntersection(const YRect &rect) const;
        bool hasPos(const YPos& other) const;
        YRect operator-(const YPos& other) const;

    private:
        int m_nMinX;
        int m_nMaxX;
        int m_nMinY;
        int m_nMaxY;
};