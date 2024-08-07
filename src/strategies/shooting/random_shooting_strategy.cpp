#include <cstdlib>

#include "random_shooting_strategy.h"
#include <cstdlib>


void RandomShootingStrategy::shoot(const YPos& p0, MoveObjectDirection direction) {
    int nX = std::rand() % 500 - 250;
    int nY = std::rand() % 500 - 250;
    m_vBioplasts.push_back(new GameBioplastState(p0, p0 + YPos(nX, nY)));
}

GameBioplastState *RandomShootingStrategy::popBioplast() {
    if (m_vBioplasts.empty()) {
        return nullptr;
    }
    GameBioplastState *pRet = m_vBioplasts.back();
    m_vBioplasts.pop_back();
    return pRet;
}