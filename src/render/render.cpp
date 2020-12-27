#include "render.h"
#include "wsjcpp_core.h"
#include <cmath>
#define PI 3.14159265

// ---------------------------------------------------------------------
// RenderObject

RenderObject::RenderObject(int nPositionZ) {
    m_nPositionZ = nPositionZ;
}

int RenderObject::getPositionZ() {
    return m_nPositionZ;
}

void RenderObject::modify(const RenderStateObjects& state) {
    // nothing modify in base object
}

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::modify(const RenderStateObjects& state) {
    m_coord1.update(
        state.getCoordLeftTop().x() + m_startCoord1.x(),
        state.getCoordLeftTop().y() + m_startCoord1.y()
    );
    m_coord2.update(
        state.getCoordLeftTop().x() + m_startCoord2.x(),
        state.getCoordLeftTop().y() + m_startCoord2.y()
    );
}

void RenderLine::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
}

const CoordXY &RenderLine::getAbsoluteCoord1() {
    return m_startCoord1;
}

const CoordXY &RenderLine::getAbsoluteCoord2() {
    return m_startCoord2;
}

void RenderLine::updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2) {
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

// ---------------------------------------------------------------------
// RenderTriangle

RenderTriangle::RenderTriangle(
    const CoordXY &p1,
    const CoordXY &p2,
    const CoordXY &p3,
    int nPositionZ
) : RenderObject(nPositionZ), m_line1(p1,p2), m_line2(p2,p3), m_line3(p3,p1) {
    // nothing
}

void RenderTriangle::modify(const RenderStateObjects& state) {
    m_line1.modify(state);
    m_line2.modify(state);
    m_line3.modify(state);
}

void RenderTriangle::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);
}


// ---------------------------------------------------------------------
// RenderTriangleAnimated1

RenderTriangleAnimated1::RenderTriangleAnimated1(
    const CoordXY &p1,
    const CoordXY &p2,
    const CoordXY &p3,
    int nPositionZ
) : RenderObject(nPositionZ), m_line1(p1,p2), m_line2(p2,p3), m_line3(p3,p1) {
    m_coordDirection = CoordXY(10, 10);
}

void RenderTriangleAnimated1::modify(const RenderStateObjects& state) {
    if (state.getElapsedTime() > 2000) {
        CoordXY p1 = m_line1.getAbsoluteCoord1();
        CoordXY p2 = m_line1.getAbsoluteCoord2();
        CoordXY p3 = m_line2.getAbsoluteCoord2();
        p1.update(p1.x() + m_coordDirection.x(), p1.y() + m_coordDirection.y());
        p2.update(p2.x() + m_coordDirection.x(), p2.y() + m_coordDirection.y());
        p3.update(p3.x() + m_coordDirection.x(), p3.y() + m_coordDirection.y());

        if (
            p1.x() < 0
            || p2.x() < 0
            || p3.x() < 0
            || p1.x() > state.windowWidth() 
            || p2.x() > state.windowWidth()
            || p3.x() > state.windowWidth()
            || p1.y() < 0
            || p2.y() < 0
            || p3.y() < 0
            || p1.y() > state.windowHeight() 
            || p2.y() > state.windowHeight()
            || p3.y() > state.windowHeight()
        ) {
            m_coordDirection.update(m_coordDirection.x()*-1, m_coordDirection.y()*-1);
        }

        m_line1.updateAbsoluteCoords(p1, p2);
        m_line2.updateAbsoluteCoords(p2, p3);
        m_line3.updateAbsoluteCoords(p3, p1);
    }

    m_line1.modify(state);
    m_line2.modify(state);
    m_line3.modify(state);
}

void RenderTriangleAnimated1::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);
}

// ---------------------------------------------------------------------
// RenderPlayer0

RenderPlayer0::RenderPlayer0(
    const CoordXY &p0,
    float nSpeedAnimation
) : RenderObject(1000) {


    m_nSpeedAnimation = nSpeedAnimation;
    m_nSize = 20;
    m_coordCenter = p0;
    CoordXY p1 = p0;
    p1 += CoordXY(-m_nSize,m_nSize);
    CoordXY p2 = p0;
    p2 += CoordXY(m_nSize,m_nSize);
    CoordXY p3 = p0;
    p3 += CoordXY(0,-m_nSize);

    m_pLine1 = new RenderLine(p1,p2);
    m_pLine2 = new RenderLine(p2,p3);
    m_pLine3 = new RenderLine(p3,p1);

    m_rectFrame.x = m_coordCenter.x() - m_nSize/4;
    m_rectFrame.y = m_coordCenter.y() - m_nSize/4;
    m_rectFrame.w = m_nSize/2;
    m_rectFrame.h = m_nSize/2;
}

void RenderPlayer0::modify(const RenderStateObjects& state) {
    float nTimePosition = float(state.getElapsedTime() / m_nSpeedAnimation);
    // nTimePosition = nTimePosition / 1000;
    int diffX1 = sin(nTimePosition) * m_nSize;
    int diffY1 = cos(nTimePosition) * m_nSize;
    
    int diffX2 = sin(nTimePosition + PI/3) * m_nSize;
    int diffY2 = cos(nTimePosition + PI/3) * m_nSize;

    int diffX3 = sin(nTimePosition + 2*PI/3) * m_nSize;
    int diffY3 = cos(nTimePosition + 2*PI/3) * m_nSize;

    CoordXY p1 = m_coordCenter;
    p1 += CoordXY(diffX1, diffY1);
    p1 -= state.getCoordLeftTop();
    CoordXY p2 = m_coordCenter;
    p2 += CoordXY(diffX2, diffY2);
    p2 -= state.getCoordLeftTop();
    CoordXY p3 = m_coordCenter;
    p3 += CoordXY(diffX3, diffY3);
    p3 -= state.getCoordLeftTop();

    // std::cout << diffX1 << " " << diffY1 << std::endl;
    m_pLine1->updateAbsoluteCoords(p1, p2);
    m_pLine2->updateAbsoluteCoords(p2, p3);
    m_pLine3->updateAbsoluteCoords(p3, p1);

    m_pLine1->modify(state);
    m_pLine2->modify(state);
    m_pLine3->modify(state);

    // rect pulsation
    float nHartPulsation = float(state.getElapsedTime() / m_nSpeedAnimation*0.5 );
    int nBorderLength = m_nSize/2;
    nBorderLength = 3 + sin(nHartPulsation)*nBorderLength; 
    m_rectFrame.x = m_coordCenter.x() - nBorderLength/2;
    m_rectFrame.y = m_coordCenter.y() - nBorderLength/2;
    m_rectFrame.w = nBorderLength;
    m_rectFrame.h = nBorderLength;
}

void RenderPlayer0::draw(SDL_Renderer* renderer) {
    m_pLine1->draw(renderer);
    m_pLine2->draw(renderer);
    m_pLine3->draw(renderer);
    SDL_RenderFillRect( renderer, &m_rectFrame);

    // SDL_RenderDrawRect(renderer, &m_rectFrame)
}

// ---------------------------------------------------------------------
// RenderRectTexture

RenderRectTexture::RenderRectTexture(const CoordXY &p0, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_coordCenter = p0;
}

void RenderRectTexture::modify(const RenderStateObjects& state) {

};

void RenderRectTexture::draw(SDL_Renderer* renderer) {

};

