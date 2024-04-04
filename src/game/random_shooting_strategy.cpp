#include "random_shooting_strategy.h"
#include "game_alien_ship_state.h"

RandomShootingStrategy::RandomShootingStrategy() {};

void RandomShootingStrategy::shoot() {
	int nX = std::rand() % 500 - 250;
	int nY = std::rand() % 500 - 250;
	m_vBioplasts.push_back(new GameBioplastState(m_p0, m_p0 + CoordXY(nX, nY)));
}
GameBioplastState *RandomShootingStrategy::popBioplast() {
	GameBioplastState *pRet = nullptr;
    if (!m_vBioplasts.empty()) {
       pRet = m_vBioplasts.back();
        m_vBioplasts.pop_back();
    }
   return pRet;
}