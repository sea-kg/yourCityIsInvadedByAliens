#include "game_tank0_state.h"

// ---------------------------------------------------------------------
// GameTank0State

GameTank0State::GameTank0State(const CoordXY &p0) 
: m_nDirection(MoveObjectDirection::UP) {
    m_p0 = p0;
    m_bHasRocket = true;
}

MoveObjectDirection GameTank0State::getDirection() {
    return m_nDirection;
}

const CoordXY &GameTank0State::getPosition() {
    return m_p0;
}

void GameTank0State::turnLeft() {
    switch (m_nDirection) {
        case MoveObjectDirection::UP:
            m_nDirection = MoveObjectDirection::UP_LEFT;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_nDirection = MoveObjectDirection::LEFT;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_nDirection = MoveObjectDirection::UP;
            break;
        case MoveObjectDirection::DOWN:
            m_nDirection = MoveObjectDirection::DOWN_RIGHT;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_nDirection = MoveObjectDirection::DOWN;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_nDirection = MoveObjectDirection::RIGHT;
            break;
        case MoveObjectDirection::LEFT:
            m_nDirection = MoveObjectDirection::DOWN_LEFT;
            break;
        case MoveObjectDirection::RIGHT:
            m_nDirection = MoveObjectDirection::UP_RIGHT;
            break;
    }
}

void GameTank0State::turnRight() {
    switch (m_nDirection) {
        case MoveObjectDirection::UP:
            m_nDirection = MoveObjectDirection::UP_RIGHT;
            break;
        case MoveObjectDirection::UP_LEFT:
            m_nDirection = MoveObjectDirection::UP;
            break;
        case MoveObjectDirection::UP_RIGHT:
            m_nDirection = MoveObjectDirection::RIGHT;
            break;
        case MoveObjectDirection::DOWN:
            m_nDirection = MoveObjectDirection::DOWN_LEFT;
            break;
        case MoveObjectDirection::DOWN_LEFT:
            m_nDirection = MoveObjectDirection::LEFT;
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            m_nDirection = MoveObjectDirection::DOWN;
            break;
        case MoveObjectDirection::LEFT:
            m_nDirection = MoveObjectDirection::UP_LEFT;
            break;
        case MoveObjectDirection::RIGHT:
            m_nDirection = MoveObjectDirection::DOWN_RIGHT;
            break;
    }
}

void GameTank0State::move() {
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

bool GameTank0State::hasRocket() {
    return m_bHasRocket;
}

void GameTank0State::shotRocket() {
    m_bHasRocket = false;
    m_vRockets.push_back(new GameRocketState(m_p0, m_nDirection));
}

void GameTank0State::rechargeRocket() {
    m_bHasRocket = true;
}

GameRocketState *GameTank0State::popRocket() {
    GameRocketState *pRet = nullptr;
    if (!m_vRockets.empty()) {
        pRet = m_vRockets.back();
        m_vRockets.pop_back();
    }
    return pRet;
}
