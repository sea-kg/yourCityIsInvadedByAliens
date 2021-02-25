#include "game_tank0_state.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// GameRocketState

GameRocketState::GameRocketState(const CoordXY &p0, MoveObjectDirection direction) 
: m_nDirection(direction) {
    m_p0 = p0;
    m_bDestroyed = false;
}

MoveObjectDirection GameRocketState::getDirection() {
    return m_nDirection;
}

const CoordXY &GameRocketState::getPosition() {
    return m_p0;
}

void GameRocketState::move() {
    int nStep = 10;
    if (m_nDirection == MoveObjectDirection::UP) {
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
    }
}

bool GameRocketState::hasDestroyed() {
    return m_bDestroyed;
}
