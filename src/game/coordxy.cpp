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