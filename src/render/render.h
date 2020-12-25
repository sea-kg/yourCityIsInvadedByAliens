#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class CoordXY {
    public:
        CoordXY();
        CoordXY(int x, int y);
        int x();
        int y();
    private:
        int m_nX, m_nY;
};

class RenderStateObjects {

    public:
        RenderStateObjects();
        void init();
        long getElapsedTime();

    private:
        long m_nElapsedTime;
        long m_nStartTime;
};

class RenderObject {

    public:
        RenderObject(int nPositionZ);
        int getPositionZ();
        virtual void modify(const RenderStateObjects& state);
        virtual void draw(SDL_Renderer* pRenderer) = 0;

    private:
        int m_nPositionZ;
};

class RenderLine : public RenderObject {

    public:
        RenderLine(const CoordXY &p1, const CoordXY &p2, int nPositionZ = 0);
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coord1;
        CoordXY m_coord2;
};

class RenderTriangle : public RenderObject {

    public:
        RenderTriangle(
            const CoordXY &p1,
            const CoordXY &p2,
            const CoordXY &p3,
            int nPositionZ = 0
        );
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        CoordXY m_coord1;
        CoordXY m_coord2;
        CoordXY m_coord3;
};