#pragma once
#include <vector>

class CoordXY {
    public:
        CoordXY();
        CoordXY(int x, int y);
        int x() const;
        int y() const;
        void update(int x, int y);

        CoordXY& operator+=(const CoordXY& other);

    private:
        int m_nX, m_nY;
};

class RenderStateObjects {

    public:
        RenderStateObjects(int windowWidth, int windowHeight);
        void init();
        void updateElapsedTime();
        long getElapsedTime() const;
        const CoordXY &getCoordLeftTop() const;
        void incrementCoordLeftTopX(int nX);
        void incrementCoordLeftTopY(int nY);
        
        const int windowWidth() const;
        const int windowHeight() const;

    private:
        long m_nElapsedTime;
        long m_nStartTime;
        CoordXY m_coordLeftTop;
        int m_nWindowWidth;
        int m_nWindowHeight;
};