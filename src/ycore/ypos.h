#pragma once

class YPos {
    public:
        YPos();
        YPos(int x, int y);
        int getX() const;
        int getY() const;
        void update(int x, int y);
        void update(const YPos& coord);
        void setX(int x);
        void setY(int y);

        YPos& operator+=(const YPos& other);
        YPos& operator-=(const YPos& other);
        YPos operator+(const YPos& other) const;
        YPos operator-(const YPos& other) const;
        bool operator>(const YPos& other) const;
        bool operator<(const YPos& other) const;

        bool isInsideRect(const YPos& topLeft, const YPos& bottomRight) const;
        float getDistance(const YPos& pos) const;

    private:
        int m_nX, m_nY;
};