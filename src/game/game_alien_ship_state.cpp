#include "game_alien_ship_state.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// GameAlienShipState

GameAlienShipState::GameAlienShipState(const CoordXY &p0) {
    m_p0 = p0;
    m_bShooting = false;
    m_nMovePrevTime = 0;
    m_nSpeedMoving = 25;
    m_moveDirection = MoveObjectDirection::NONE;
}

const CoordXY &GameAlienShipState::getPosition() {
    return m_p0;
}

void GameAlienShipState::setShooting(bool bShooting) {
    m_bShooting = bShooting;
}

bool GameAlienShipState::isShooting() const {
    return m_bShooting;
}

void GameAlienShipState::setMoveDirection(MoveObjectDirection direction) {
    m_moveDirection = direction;
}

void GameAlienShipState::move(long nElapsedTime) {
    long position = nElapsedTime / m_nSpeedMoving;

    if (m_nMovePrevTime == position) {
        return; // skip - already desition done
    }

    m_nMovePrevTime = position;

    int nStep = 5;
    switch(m_moveDirection) {
        case MoveObjectDirection::UP:
            m_p0 = m_p0 + CoordXY(0, -1*nStep);
            break;
        case MoveObjectDirection::UP_LEFT:
            m_p0 = m_p0 + CoordXY(-1*nStep, -1*nStep);
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_p0 = m_p0 + CoordXY(nStep, -1*nStep);
            break;
        case MoveObjectDirection::DOWN:
            m_p0 = m_p0 + CoordXY(0, nStep);
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_p0 = m_p0 + CoordXY(-1*nStep, nStep);
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_p0 = m_p0 + CoordXY(nStep, nStep);
            break;
        case MoveObjectDirection::LEFT:
            m_p0 = m_p0 + CoordXY(-1*nStep, 0);
            break;
        case MoveObjectDirection::RIGHT:
            m_p0 = m_p0 + CoordXY(nStep, 0);
            break;
    }
}

void GameAlienShipState::shot() {
    m_vBioplasts.push_back(new GameBioplastState(m_p0, m_p0 + CoordXY(0,250)));
}

GameBioplastState *GameAlienShipState::popRocket() {
    GameBioplastState *pRet = nullptr;
    if (!m_vBioplasts.empty()) {
        pRet = m_vBioplasts.back();
        m_vBioplasts.pop_back();
    }
    return pRet;
}
