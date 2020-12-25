#include "render.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// CoordXY

CoordXY::CoordXY() {
    m_nX = 0;
    m_nY = 0;
}

CoordXY::CoordXY(int x, int y) {
    m_nX = x;
    m_nY = y;
}

int CoordXY::x() const {
    return m_nX;
}

int CoordXY::y() const {
    return m_nY;
}

void CoordXY::update(int x, int y) {
    m_nX = x;
    m_nY = y;
}

// ---------------------------------------------------------------------
// RenderStateObjects

RenderStateObjects::RenderStateObjects(int windowWidth, int windowHeight) {
    m_nStartTime = 0;
    m_nElapsedTime = 0;
    m_nWindowWidth = windowWidth;
    m_nWindowHeight = windowHeight;
}

void RenderStateObjects::init() {
    m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
}

void RenderStateObjects::updateElapsedTime() {
    m_nElapsedTime = WsjcppCore::getCurrentTimeInMilliseconds() - m_nStartTime;
}

long RenderStateObjects::getElapsedTime() const {
    return m_nElapsedTime;
}

const CoordXY &RenderStateObjects::getCoordLeftTop() const {
    return m_coordLeftTop;
}

const int RenderStateObjects::windowWidth() const {
    return m_nWindowWidth;
}

const int RenderStateObjects::windowHeight() const {
    return m_nWindowHeight;
}

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