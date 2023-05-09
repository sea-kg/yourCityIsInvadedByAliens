#include "game_rocket_state.h"

// ---------------------------------------------------------------------
// GameRocketState

GameRocketState::GameRocketState(const YPos &p0, MoveObjectDirection direction) 
: m_nDirection(direction) {
    m_p0 = p0;
    m_bDestroyed = false;
    m_bExploded = false;
    m_bCanBeRemoved = false;
}

MoveObjectDirection GameRocketState::getDirection() {
    return m_nDirection;
}

const YPos &GameRocketState::getPosition() {
    return m_p0;
}

void GameRocketState::move() {
    int nStep = 10;
    if (m_nDirection == MoveObjectDirection::UP) {
        m_p0 += YPos(0,-1 * nStep);
    } else if (m_nDirection == MoveObjectDirection::UP_LEFT) {
        m_p0 += YPos(-1*nStep, -1*nStep);
    } else if (m_nDirection == MoveObjectDirection::UP_RIGHT) {
        m_p0 += YPos(nStep, -1*nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN) {
        m_p0 += YPos(0, nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN_LEFT) {
        m_p0 += YPos(-1*nStep, nStep);
    } else if (m_nDirection == MoveObjectDirection::DOWN_RIGHT) {
        m_p0 += YPos(nStep, nStep);
    } else if (m_nDirection == MoveObjectDirection::LEFT) {
        m_p0 += YPos(-1*nStep, 0);
    } else if (m_nDirection == MoveObjectDirection::RIGHT) {
        m_p0 += YPos(nStep, 0);
    }
}

bool GameRocketState::hasDestroyed() {
    return m_bDestroyed;
}

void GameRocketState::destroy() {
    m_bDestroyed = true;
}

void GameRocketState::explode() {
    m_bExploded = true;
}

bool GameRocketState::isExploded() {
    return m_bExploded;
}

bool GameRocketState::canBeRemoved() {
    return m_bCanBeRemoved;
}

void GameRocketState::removeLater() {
    m_bCanBeRemoved = true;
}