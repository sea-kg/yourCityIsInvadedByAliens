#include "game_alien_ship_state.h"
#include <iostream>

// ---------------------------------------------------------------------
// GameAlienShipState

GameAlienShipState::GameAlienShipState(const CoordXY &p0) {
    m_p0 = p0;
    m_bShooting = false;
    m_nMovePrevTime = 0;
    m_nSpeedMoving = 25;
    m_nMaxHealthPoints = 64;
    m_nHealthPoints = m_nMaxHealthPoints;
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

void GameAlienShipState::move(
    long nElapsedTime,
    const CoordXY &minPointMap,
    const CoordXY &maxPointMap,
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
    CoordXY p0;
    switch(m_moveDirection) {
        case MoveObjectDirection::UP:
            p0 = m_p0 + CoordXY(0, -1*nStep);
            break;
        case MoveObjectDirection::UP_LEFT:
            p0 = m_p0 + CoordXY(-1*nStep, -1*nStep);
            break;
        case MoveObjectDirection::UP_RIGHT:
            p0 = m_p0 + CoordXY(nStep, -1*nStep);
            break;
        case MoveObjectDirection::DOWN:
            p0 = m_p0 + CoordXY(0, nStep);
            break;
        case MoveObjectDirection::DOWN_LEFT:
            p0 = m_p0 + CoordXY(-1*nStep, nStep);
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            p0 = m_p0 + CoordXY(nStep, nStep);
            break;
        case MoveObjectDirection::LEFT:
            p0 = m_p0 + CoordXY(-1*nStep, 0);
            break;
        case MoveObjectDirection::RIGHT:
            p0 = m_p0 + CoordXY(nStep, 0);
            break;
        default:
            p0 = m_p0;
    }
    // game map borders
    if (p0.x() > maxPointMap.x() - nRightPad) {
        p0.setX(maxPointMap.x() - nRightPad);
    }
    if (p0.x() < minPointMap.x() + nLeftPad) {
        p0.setX(minPointMap.x() + nLeftPad);
    }
    if (p0.y() > maxPointMap.y() - nBottomPad) {
        p0.setY(maxPointMap.y() - nBottomPad);
    }
    if (p0.y() < minPointMap.y() + nTopPad) {
        p0.setY(minPointMap.y() + nTopPad);
    }
    m_p0.update(p0);
}

void GameAlienShipState::bioplastShot() {
    int nX = std::rand() % 500 - 250;
    int nY = std::rand() % 500 - 250;
    m_vBioplasts.push_back(new GameBioplastState(m_p0, m_p0 + CoordXY(nX,nY)));
}

GameBioplastState *GameAlienShipState::popBioplast() {
    GameBioplastState *pRet = nullptr;
    if (!m_vBioplasts.empty()) {
        pRet = m_vBioplasts.back();
        m_vBioplasts.pop_back();
    }
    return pRet;
}

void GameAlienShipState::rocketAttack(GameRocketState *pRocket) {
    std::cout << "GameAlienShipState::rocketAttack, negative hit points" << std::endl;
    m_nHealthPoints--;
    MoveObjectDirection rocketMoveDirection = pRocket->getDirection();

    // move ship after rocket 
    int nStep = 15;
    CoordXY p0;
    switch(rocketMoveDirection) {
        case MoveObjectDirection::UP:
            p0 = m_p0 + CoordXY(0, -1*nStep);
            break;
        case MoveObjectDirection::UP_LEFT:
            p0 = m_p0 + CoordXY(-1*nStep, -1*nStep);
            break;
        case MoveObjectDirection::UP_RIGHT:
            p0 = m_p0 + CoordXY(nStep, -1*nStep);
            break;
        case MoveObjectDirection::DOWN:
            p0 = m_p0 + CoordXY(0, nStep);
            break;
        case MoveObjectDirection::DOWN_LEFT:
            p0 = m_p0 + CoordXY(-1*nStep, nStep);
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            p0 = m_p0 + CoordXY(nStep, nStep);
            break;
        case MoveObjectDirection::LEFT:
            p0 = m_p0 + CoordXY(-1*nStep, 0);
            break;
        case MoveObjectDirection::RIGHT:
            p0 = m_p0 + CoordXY(nStep, 0);
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
int GameAlienShipState::setHealthPoints(int h)
{
    m_nHealthPoints = h;
    return m_nHealthPoints;
}

int GameAlienShipState::setMaxHealthPoints(int h)
{
    m_nMaxHealthPoints = h;
    return m_nMaxHealthPoints;
}
void GameAlienShipState::resetHealthPoints()
{
    m_nHealthPoints = m_nMaxHealthPoints;

}

void GameAlienShipState::updatePosition(const CoordXY &p0) {
    m_p0 = p0;
}

void GameAlienShipState::updateStateByKeyboard(YKeyboard *pKeyboard) {
    if (pKeyboard->isSpace()) {
        this->setShooting(true);
    } else if (pKeyboard->isL() || pKeyboard->isH()) {

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
        this->setMoveDirection(MoveObjectDirection::DOWN_RIGHT);
    } else if (pKeyboard->isLeft()) {
        this->setMoveDirection(MoveObjectDirection::LEFT);
    } else if (pKeyboard->isRight()) {
        this->setMoveDirection(MoveObjectDirection::RIGHT);
    } else {
        this->setMoveDirection(MoveObjectDirection::NONE);
    }
}