#include <cstdlib>

#include "random_shooting_strategy.h"
#include <cstdlib>


void RandomShootingStrategy::shoot(const CoordXY& p0) {
	int nX = std::rand() % 500 - 250;
	int nY = std::rand() % 500 - 250;
	m_vBioplasts.push_back(new GameBioplastState(p0, p0 + CoordXY(nX, nY)));
}

GameBioplastState *RandomShootingStrategy::popBioplast() {
	GameBioplastState *pRet = nullptr;
    if (!m_vBioplasts.empty()) {
       pRet = m_vBioplasts.back();
        m_vBioplasts.pop_back();
    }
   return pRet;
}