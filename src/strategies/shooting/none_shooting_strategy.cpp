#include "none_shooting_strategy.h"
#include <cstdlib>

void NoneShootingStrategy::shoot(const CoordXY& p0) {
    // nothing to do
}

GameBioplastState* NoneShootingStrategy::popBioplast() {
    GameBioplastState* pRet = nullptr;
    if (!m_vBioplasts.empty()) {
        pRet = m_vBioplasts.back();
        m_vBioplasts.pop_back();
    }
    return pRet;
}