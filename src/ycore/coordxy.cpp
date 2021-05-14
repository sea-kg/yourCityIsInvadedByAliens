#include "coordxy.h"

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

void CoordXY::update(const CoordXY& coord) {
    m_nX = coord.x();
    m_nY = coord.y();
}

void CoordXY::setX(int x) {
    m_nX = x;
}

void CoordXY::setY(int y) {
    m_nY = y;
}

YPos CoordXY::toYPos() const {
    return YPos(m_nX, m_nY);
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

CoordXY CoordXY::operator+(const CoordXY& other) const {
    CoordXY c(m_nX, m_nY);
    c += other;
    return c;
}

CoordXY CoordXY::operator-(const CoordXY& other) const {
    CoordXY c(m_nX, m_nY);
    c -= other;
    return c;
}

bool CoordXY::operator>(const CoordXY& other) const {
    return m_nX > other.x() || m_nY > other.y();
}

bool CoordXY::operator<(const CoordXY& other) const {
    return m_nX < other.x() || m_nY < other.y();
}

bool CoordXY::isInsideRect(const CoordXY& topLeft, const CoordXY& bottomRight) const {
    return
        m_nX >= topLeft.x()
        && m_nX <= bottomRight.x()
        && m_nY >= topLeft.y()
        && m_nY <= bottomRight.y()
    ;
}

bool CoordXY::isInsideRect(const YRect &rect) {
    return
           m_nX >= rect.getMinX()
        && m_nX <= rect.getMaxX()
        && m_nY >= rect.getMinY()
        && m_nY <= rect.getMaxY()
    ;
}
