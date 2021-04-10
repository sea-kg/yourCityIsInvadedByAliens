#pragma once

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

        CoordXY& operator+=(const CoordXY& other);
        CoordXY& operator-=(const CoordXY& other);
        CoordXY operator+(const CoordXY& other) const;
        CoordXY operator-(const CoordXY& other) const;

        bool isInsideRect(const CoordXY& topLeft, const CoordXY& bottomRight) const;
    private:
        int m_nX, m_nY;
};