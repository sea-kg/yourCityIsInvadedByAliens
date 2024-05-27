#include "game_tank0_state.h"
#include "ylog.h"
// #include <string>
// #include <sstream>
// #include <codecvt>

// ---------------------------------------------------------------------
// GameTank0State

GameTank0State::GameTank0State(const YPos &p0)
: m_nDirection(MoveObjectDirection::UP) {
    TAG = L"GameTank0State";
    m_p0 = p0;
    m_bHasRocket = true;
    m_nMoveStep = 10;

    m_vMoveVectors.resize(9);
    m_vMoveVectors[int(MoveObjectDirection::NONE)] = YPos(0, 0);
    m_vMoveVectors[int(MoveObjectDirection::UP)] = YPos(0, -1);
    m_vMoveVectors[int(MoveObjectDirection::DOWN)] = YPos(0, 1);
    m_vMoveVectors[int(MoveObjectDirection::UP_LEFT)] = YPos(-1, -1);
    m_vMoveVectors[int(MoveObjectDirection::DOWN_LEFT)] = YPos(-1, 1);
    m_vMoveVectors[int(MoveObjectDirection::UP_RIGHT)] = YPos(1, -1);
    m_vMoveVectors[int(MoveObjectDirection::DOWN_RIGHT)] = YPos(1, 1);
    m_vMoveVectors[int(MoveObjectDirection::LEFT)] = YPos(-1, 0);
    m_vMoveVectors[int(MoveObjectDirection::RIGHT)] = YPos(1, 0);
}

MoveObjectDirection GameTank0State::getDirection() {
    return m_nDirection;
}

const YPos &GameTank0State::getPosition() {
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

YPos GameTank0State::calculateMoveForward() {
    const YPos &vMove = m_vMoveVectors[int(m_nDirection)];
    YPos xy = m_p0 + YPos(vMove.getX() * m_nMoveStep, vMove.getY() * m_nMoveStep);
    return xy;
}

void GameTank0State::moveForward() {
    m_p0 = calculateMoveForward();
}

bool GameTank0State::hasRocket() {
    return m_bHasRocket;
}

void GameTank0State::shotRocket() {
    m_bHasRocket = false;
    m_vRockets.push_back(new GameRocketState(YPos(m_p0.getX(), m_p0.getY()), m_nDirection));
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
