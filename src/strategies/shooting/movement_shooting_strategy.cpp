#include <cstdlib>

#include "movement_shooting_strategy.h"
#include "ylog.h"

void MovementShootingStrategy::shoot(const YPos& p0, MoveObjectDirection direction) {

    int nX = 0;
    int nY = 0;

    switch (direction) {
        case MoveObjectDirection::NONE:
            nX = 0;
            nY = 0;
            break;
        case MoveObjectDirection::UP:
            nX = 0;
            nY = 500;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::LEFT:
            nX = 500;
            nY = 0;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::RIGHT:
            nX = -500;
            nY = 0;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::DOWN:
            nX = 0;
            nY = -500;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::UP_LEFT:
            nX = 500;
            nY = 500;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::UP_RIGHT:
            nX = -500;
            nY = 500;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::DOWN_LEFT:
            nX = 500;
            nY = -500;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        case MoveObjectDirection::DOWN_RIGHT:
            nX = -500;
            nY = -500;
            m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
            break;
        default:
            YLog::err(L"MovementShootingStrategy::shoot", L"Wrong direction");
            break;
    }
}

GameBioplastState *MovementShootingStrategy::popBioplast() {
    if (m_vBioplasts.empty()) {
        return nullptr;
    }
    GameBioplastState *pRet = m_vBioplasts.back();
    m_vBioplasts.pop_back();
    return pRet;
}