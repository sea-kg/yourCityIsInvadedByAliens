#include "yrect.h"
#include <algorithm>

// ---------------------------------------------------------------------
// YRect

YRect::YRect() {
    m_nMinX = 0;
    m_nMaxX = 0;
    m_nMinY = 0;
    m_nMaxY = 0;
}

YRect::YRect(const YPos& minpos, const YPos& maxpos) {
    m_nMinX = std::min(minpos.getX(), maxpos.getX());
    m_nMinY = std::min(minpos.getY(), maxpos.getY());
    m_nMaxX = std::max(minpos.getX(), maxpos.getX());
    m_nMaxY = std::max(minpos.getY(), maxpos.getY());
}

const int &YRect::getMinX() const {
    return m_nMinX;
}

const int &YRect::getMaxX() const {
    return m_nMaxX;
}

const int &YRect::getMinY() const {
    return m_nMinY;
}

const int &YRect::getMaxY() const {
    return m_nMaxY;
}

bool YRect::hasPos(const YPos& other) const {
    return
        m_nMinX <= other.getX() && other.getX() <= m_nMaxX
        && m_nMinY <= other.getY() && other.getY() <= m_nMaxY
    ;
}

bool YRect::hasIntersection(const YRect &rect) const {
    return
        (
               (rect.getMinX() <= m_nMinX && m_nMinX <= rect.getMaxX())
            || (rect.getMinX() <= m_nMaxX && m_nMaxX <= rect.getMaxX())
        ) && (
               (rect.getMinY() <= m_nMinY && m_nMinY <= rect.getMaxY())
            || (rect.getMinY() <= m_nMaxY && m_nMaxY <= rect.getMaxY())
        )
    ;
}

YRect YRect::operator-(const YPos& other) const {
    return YRect(
        YPos(
            m_nMinX - other.getX(),
            m_nMinY - other.getY()
        ),
        YPos(
            m_nMaxX - other.getX(),
            m_nMaxY - other.getY()
        )
    );
}
