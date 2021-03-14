#include "game_alien_ship_state.h"
#include "wsjcpp_core.h"

// ---------------------------------------------------------------------
// GameAlienShipState

GameAlienShipState::GameAlienShipState(const CoordXY &p0) {
    m_p0 = p0;
    m_bShooting = false;
}

const CoordXY &GameAlienShipState::getPosition() {
    return m_p0;
}

void GameAlienShipState::shot() {
    m_vBioplasts.push_back(new GameBioplastState(m_p0, m_p0 + CoordXY(0,250)));
}

void GameAlienShipState::setShooting(bool bShooting) {
    m_bShooting = bShooting;
}

bool GameAlienShipState::isShooting() const {
    return m_bShooting;
}

GameBioplastState *GameAlienShipState::popRocket() {
    GameBioplastState *pRet = nullptr;
    if (!m_vBioplasts.empty()) {
        pRet = m_vBioplasts.back();
        m_vBioplasts.pop_back();
    }
    return pRet;
}
