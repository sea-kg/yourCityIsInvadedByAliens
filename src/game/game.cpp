#include "game.h"
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

CoordXY& CoordXY::operator+=(const CoordXY& other) {
    m_nX = m_nX + other.x();
    m_nY = m_nY + other.y();
    return *this;
}

CoordXY& CoordXY::operator-=(const CoordXY& other) {
    m_nX = m_nX - other.x();
    m_nY = m_nY - other.y();
    return *this;
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

void RenderStateObjects::incrementCoordLeftTopX(int nX) {
    m_coordLeftTop += CoordXY(nX, 0);
}

void RenderStateObjects::incrementCoordLeftTopY(int nY) {
    m_coordLeftTop += CoordXY(0, nY);
}

const int RenderStateObjects::windowWidth() const {
    return m_nWindowWidth;
}

const int RenderStateObjects::windowHeight() const {
    return m_nWindowHeight;
}