#include "game_alien_ship_state.h"
#include <iostream>
#include "ylog.h"
#include "random_shooting_strategy.h"
#include "movement_shooting_strategy.h"

// ---------------------------------------------------------------------
// GameAlienShipState

GameAlienShipState::GameAlienShipState(const YPos &p0) {
    m_p0 = p0;
    m_bShooting = false;
    m_nMovePrevTime = 0;
    m_nSpeedMoving = 25;
    m_nMaxHealthPoints = 64;
    m_nHealthPoints = m_nMaxHealthPoints;
    m_moveDirection = MoveObjectDirection::NONE;
    m_pShootingStrategyLogic = new ShootingStrategyLogic();
}

const YPos &GameAlienShipState::getPosition() {
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

MoveObjectDirection GameAlienShipState::getMoveDirection() const {
    return m_moveDirection;
}

void GameAlienShipState::move(
    long nElapsedTime,
    const YPos &minPointMap,
    const YPos &maxPointMap,
    int nLeftPad,
    int nRightPad,
    int nTopPad,
    int nBottomPad
) {
    long position = nElapsedTime / m_nSpeedMoving;

    if (m_nMovePrevTime == position) {
        return; // skip - already desition done
    }

    m_nMovePrevTime = position;

    int nStep = 5;
    YPos p0;
    switch(m_moveDirection) {
        case MoveObjectDirection::UP:
            p0 = m_p0 + YPos(0, -1*nStep);
            break;
        case MoveObjectDirection::UP_LEFT:
            p0 = m_p0 + YPos(-1*nStep, -1*nStep);
            break;
        case MoveObjectDirection::UP_RIGHT:
            p0 = m_p0 + YPos(nStep, -1*nStep);
            break;
        case MoveObjectDirection::DOWN:
            p0 = m_p0 + YPos(0, nStep);
            break;
        case MoveObjectDirection::DOWN_LEFT:
            p0 = m_p0 + YPos(-1*nStep, nStep);
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            p0 = m_p0 + YPos(nStep, nStep);
            break;
        case MoveObjectDirection::LEFT:
            p0 = m_p0 + YPos(-1*nStep, 0);
            break;
        case MoveObjectDirection::RIGHT:
            p0 = m_p0 + YPos(nStep, 0);
            break;
        default:
            p0 = m_p0;
    }
    // game map borders
    if (p0.getX() > maxPointMap.getX() - nRightPad) {
        p0.setX(maxPointMap.getX() - nRightPad);
    }
    if (p0.getX() < minPointMap.getX() + nLeftPad) {
        p0.setX(minPointMap.getX() + nLeftPad);
    }
    if (p0.getY() > maxPointMap.getY() - nBottomPad) {
        p0.setY(maxPointMap.getY() - nBottomPad);
    }
    if (p0.getY() < minPointMap.getY() + nTopPad) {
        p0.setY(minPointMap.getY() + nTopPad);
    }
    m_p0.update(p0);
}

void GameAlienShipState::rocketAttack(GameRocketState *pRocket) {
    std::cout << "GameAlienShipState::rocketAttack, negative hit points" << std::endl;
    m_nHealthPoints--;
    MoveObjectDirection rocketMoveDirection = pRocket->getDirection();

    // move ship after rocket
    int nStep = 15;
    YPos p0;
    switch(rocketMoveDirection) {
        case MoveObjectDirection::UP:
            p0 = m_p0 + YPos(0, -1*nStep);
            break;
        case MoveObjectDirection::UP_LEFT:
            p0 = m_p0 + YPos(-1*nStep, -1*nStep);
            break;
        case MoveObjectDirection::UP_RIGHT:
            p0 = m_p0 + YPos(nStep, -1*nStep);
            break;
        case MoveObjectDirection::DOWN:
            p0 = m_p0 + YPos(0, nStep);
            break;
        case MoveObjectDirection::DOWN_LEFT:
            p0 = m_p0 + YPos(-1*nStep, nStep);
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            p0 = m_p0 + YPos(nStep, nStep);
            break;
        case MoveObjectDirection::LEFT:
            p0 = m_p0 + YPos(-1*nStep, 0);
            break;
        case MoveObjectDirection::RIGHT:
            p0 = m_p0 + YPos(nStep, 0);
            break;
        default:
            p0 = m_p0;
    }
    m_p0.update(p0);

}

int GameAlienShipState::getHelthPoints() {
    return m_nHealthPoints;
}

int GameAlienShipState::getMaxHelthPoints() {
    return m_nMaxHealthPoints;
}

int GameAlienShipState::setHealthPoints(int h) {
    m_nHealthPoints = h;
    return m_nHealthPoints;
}

int GameAlienShipState::setMaxHealthPoints(int h) {
    m_nMaxHealthPoints = h;
    return m_nMaxHealthPoints;
}

void GameAlienShipState::resetHealthPoints() {
    m_nHealthPoints = m_nMaxHealthPoints;
}

void GameAlienShipState::updatePosition(const YPos &p0) {
    m_p0 = p0;
}

void GameAlienShipState::updateStateByKeyboard(YKeyboard *pKeyboard) {
    if (pKeyboard->isSpace() || pKeyboard->isH()) {
        // pKeyboard->toLogPressedButtons();
        this->setShooting(true);
    } else {
        this->setShooting(false);
    }

    if (pKeyboard->isUp()) {
        this->setMoveDirection(MoveObjectDirection::UP);
    } else if (pKeyboard->isUpLeft()) {
        this->setMoveDirection(MoveObjectDirection::UP_LEFT);
    } else if (pKeyboard->isUpRight()) {
        this->setMoveDirection(MoveObjectDirection::UP_RIGHT);
    } else if (pKeyboard->isDown()) {
        this->setMoveDirection(MoveObjectDirection::DOWN);
    } else if (pKeyboard->isDownLeft()) {
        this->setMoveDirection(MoveObjectDirection::DOWN_LEFT);
    } else if (pKeyboard->isDownRight()) {
        // YLog::info(L"updateStateByKeyboard", L"MoveObjectDirection::DOWN_RIGHT");
        this->setMoveDirection(MoveObjectDirection::DOWN_RIGHT);
    } else if (pKeyboard->isLeft()) {
        this->setMoveDirection(MoveObjectDirection::LEFT);
    } else if (pKeyboard->isRight()) {
        this->setMoveDirection(MoveObjectDirection::RIGHT);
    } else {
        this->setMoveDirection(MoveObjectDirection::NONE);
    }
}

ShootingStrategyLogic *GameAlienShipState::getShootingStrategyLogic() const {
    return m_pShootingStrategyLogic;
}
