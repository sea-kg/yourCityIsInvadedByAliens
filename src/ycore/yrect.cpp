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

YRect::YRect(int nX0, int nY0, int nX1, int nY1) {
    m_nMinX = std::min(nX0, nX1);
    m_nMinY = std::min(nY0, nY1);
    m_nMaxX = std::max(nX0, nX1);
    m_nMaxY = std::max(nY0, nY1);
    // m_nWidth = m_nMaxX - m_nMinX;
    // m_nHeight = m_nMaxY - m_nMinY;
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
    /*
            Variant 1
    (X00,Y00) -------------- (X01,Y00)
        |                        |
        |    (X10,Y10) --------------- (X11,Y10)
        |        |               |         |
        |        |               |         |
    (X00,Y01) -------------- (X01,Y01)     |
                 |                         |
             (X10,Y11) --------------- (X11,Y11)

            ...

            Variant 4
            (X00,Y00) ------ (X01,Y00)
                |                |
    (X10,Y10) ---------------------- (X11,Y10)
        |       |                |       |
        |       |                |       |
    (X10,Y11) ---------------------- (X11,Y11)
                |                |
            (X00,Y01) ------ (X01,Y01)


            Variant 5
            (X10,Y10) ----- (X11,Y10)
                |               |
    (X00,Y00) --|---------------|-- (X01,Y00)
        |       |               |       |
        |       |               |       |
        |       |               |       |
    (X10,Y11) --------------------- (X11,Y11)
                |               |
            (X00,Y01) ----- (X01,Y01)
    */
    // LessOrEqual
    // bool bLoeX10X00 = rect.getMinX() <= m_nMinX;
    // bool bLoeX10X01 = rect.getMinX() <= m_nMaxX;
    // bool bLoeX11X00 = rect.getMaxX() <= m_nMinX;
    // bool bLoeX11X01 = rect.getMaxX() <= m_nMaxX;
    // bool bLoeY10Y00 = rect.getMinY() <= m_nMinY;
    // bool bLoeY10Y01 = rect.getMinY() <= m_nMaxY;
    // bool bLoeY11Y01 = rect.getMaxY() <= m_nMaxY;

    // return
    //     (!bLoeX10X00 && bLoeX10X01 && !bLoeY10Y00 && bLoeY10Y01) // Variant 1
    //     // rect.hasPos(YPos(m_nMinX, m_nMinY))
    //     || rect.hasPos(YPos(m_nMinX, m_nMaxY))
    //     || rect.hasPos(YPos(m_nMaxX, m_nMinY))
    //     || rect.hasPos(YPos(m_nMaxX, m_nMaxY))
    //     || (bLoeX10X00 && !bLoeX11X01 && (!bLoeY10Y00 && bLoeY11Y01)) // Variant 4
    //     || (!bLoeX10X00 && bLoeX11X01 && (bLoeY10Y00 && !bLoeY11Y01)) // Variant 5
    // ;

    const bool isXintersected = rect.getMinX() <= m_nMaxX && rect.getMaxX() >= m_nMinX;
    const bool isYintersected = rect.getMinY() <= m_nMaxY && rect.getMaxY() >= m_nMinY;
    return isXintersected && isYintersected;
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
