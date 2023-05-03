#include "game_alien_berry_state.h"

// ---------------------------------------------------------------------
// GameAlienBerryState

GameAlienBerryState::GameAlienBerryState(const YPos &p0, int nWidth, int nHeight) {
    m_p0 = p0;
    m_bDestroyed = false;
    m_bExploded = false;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
}

const YPos &GameAlienBerryState::getPosition() {
    return m_p0;
}

bool GameAlienBerryState::hasDestroyed() {
    return m_bDestroyed;
}

void GameAlienBerryState::destroy() {
    m_bDestroyed = true;
}

void GameAlienBerryState::explode() {
    m_bExploded = true;
}

bool GameAlienBerryState::isExploded() {
    return m_bExploded;
}

bool GameAlienBerryState::hasPoint(int x, int y) {
    return
        (x >= m_p0.getX())
        && (x <= (m_p0.getX() + m_nWidth))
        && (y >= m_p0.getY())
        && (y <= (m_p0.getY() + m_nHeight))
    ;
}

int GameAlienBerryState::getWidth() const {
    return m_nWidth;
}

int GameAlienBerryState::getHeight() const {
    return m_nHeight;
}
