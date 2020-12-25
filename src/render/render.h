#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

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
        RenderLine(int x1, int y1, int x2, int y2, int nPositionZ = 0);
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        int m_beginX1;
        int m_beginY1;
        int m_beginX2;
        int m_beginY2;
};