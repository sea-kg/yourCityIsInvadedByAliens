#include "ypos.h"
#include <cmath>
// ---------------------------------------------------------------------
// YPos

YPos::YPos() {
    m_nX = 0;
    m_nY = 0;
}

YPos::YPos(int x, int y) {
    m_nX = x;
    m_nY = y;
}

int YPos::getX() const {
    return m_nX;
}

int YPos::getY() const {
    return m_nY;
}

void YPos::update(int x, int y) {
    m_nX = x;
    m_nY = y;
}

void YPos::update(const YPos& coord) {
    m_nX = coord.getX();
    m_nY = coord.getY();
}

void YPos::setX(int x) {
    m_nX = x;
}

void YPos::setY(int y) {
    m_nY = y;
}

YPos& YPos::operator+=(const YPos& other) {
    m_nX = m_nX + other.getX();
    m_nY = m_nY + other.getY();
    return *this;
}

YPos& YPos::operator-=(const YPos& other) {
    m_nX = m_nX - other.getX();
    m_nY = m_nY - other.getY();
    return *this;
}

YPos YPos::operator+(const YPos& other) const {
    YPos c(m_nX, m_nY);
    c += other;
    return c;
}

YPos YPos::operator-(const YPos& other) const {
    YPos c(m_nX, m_nY);
    c -= other;
    return c;
}

bool YPos::operator>(const YPos& other) const {
    return m_nX > other.getX() || m_nY > other.getY();
}

bool YPos::operator<(const YPos& other) const {
    return m_nX < other.getX() || m_nY < other.getY();
}

bool YPos::isInsideRect(const YPos& topLeft, const YPos& bottomRight) const {
    return
        m_nX >= topLeft.getX()
        && m_nX <= bottomRight.getX()
        && m_nY >= topLeft.getY()
        && m_nY <= bottomRight.getY()
    ;
}

float YPos::getDistance(const YPos& pos) const {
    float dx = m_nX - pos.getX();
    float dy = m_nY - pos.getY();
    return sqrt(dx * dx + dy * dy);
}