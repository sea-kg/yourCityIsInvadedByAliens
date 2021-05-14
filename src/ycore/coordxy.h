#pragma once

#include "yrect.h"

class CoordXY {
    public:
        CoordXY();
        CoordXY(int x, int y);
        int x() const;
        int y() const;
        void update(int x, int y);
        void update(const CoordXY& coord);
        void setX(int x);
        void setY(int y);
        YPos toYPos() const;
        CoordXY& operator+=(const CoordXY& other);
        CoordXY& operator-=(const CoordXY& other);
        CoordXY operator+(const CoordXY& other) const;
        CoordXY operator-(const CoordXY& other) const;
        bool operator>(const CoordXY& other) const;
        bool operator<(const CoordXY& other) const;

        bool isInsideRect(const CoordXY& topLeft, const CoordXY& bottomRight) const;
        bool isInsideRect(const YRect &rect);
    private:
        int m_nX, m_nY;
};