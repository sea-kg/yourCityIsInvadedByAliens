#include "none_shooting_strategy.h"
#include <cstdlib>

void NoneShootingStrategy::shoot(const YPos&, MoveObjectDirection) {
    //nothing to do
}

GameBioplastState* NoneShootingStrategy::popBioplast() {
    if (m_vBioplasts.empty()) {
        return nullptr;
    }
    GameBioplastState *pRet = m_vBioplasts.back();
    m_vBioplasts.pop_back();
    return pRet;
}