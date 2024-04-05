#include "shooting_up_strategy.h"

void ShootingUpStrategy::shoot(const CoordXY& p0) {
	int nX = std::rand() % 100 - 50;
	int nY = std::rand() % 500;
	m_vBioplasts.push_back(new GameBioplastState(p0, p0 + CoordXY(nX, nY)));
}

GameBioplastState *ShootingUpStrategy::popBioplast() {
	GameBioplastState *pRet = nullptr;
	if (!m_vBioplasts.empty()) {
		pRet = m_vBioplasts.back();
		m_vBioplasts.pop_back();
	}
	return pRet;
}