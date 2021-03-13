#include "game_bioplast_state.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// GameBioplastState

GameBioplastState::GameBioplastState(const CoordXY &pStart, const CoordXY &pEnd) {
    m_p0 = pStart;
    m_pStart = pStart;
    m_pEnd = pEnd;
    m_bDestroyed = false;
}

const CoordXY &GameBioplastState::getPosition() {
    return m_p0;
}

void GameBioplastState::move() {
    int nStep = 10;

    /*if (m_nDirection == MoveObjectDirection::UP) {
        m_p0 += CoordXY(0,-1 * nStep);
    } else if (m_nDirection == MoveObjectDirection::UP_LEFT) {
        m_p0 += CoordXY(-1*nStep, -1*nStep);
    } else if (m_nDirection == MoveObjectDirection::UP_RIGHT) {
        m_p0 += CoordXY(nStep, -1*nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN) {
        m_p0 += CoordXY(0, nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN_LEFT) {
        m_p0 += CoordXY(-1*nStep, nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN_RIGHT) {
        m_p0 += CoordXY(nStep, nStep);
    } else if (m_nDirection == MoveObjectDirection::LEFT) {
        m_p0 += CoordXY(-1*nStep, 0);
    } else if (m_nDirection == MoveObjectDirection::RIGHT) {
        m_p0 += CoordXY(nStep, 0);
    }*/
}

bool GameBioplastState::hasDestroyed() {
    return m_bDestroyed;
}

void GameBioplastState::destroy() {
    m_bDestroyed = true;
}