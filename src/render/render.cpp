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

int CoordXY::x() {
    return m_nX;
}

int CoordXY::y() {
    return m_nY;
}

// ---------------------------------------------------------------------
// RenderStateObjects

RenderStateObjects::RenderStateObjects() {
    m_nStartTime = 0;
    m_nElapsedTime = 0;
}

void RenderStateObjects::init() {
    m_nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
}

long RenderStateObjects::getElapsedTime() {
    return m_nElapsedTime;
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
}

void RenderLine::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
}