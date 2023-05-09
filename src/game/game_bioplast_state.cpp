#include "game_bioplast_state.h"
#include <cmath>

// ---------------------------------------------------------------------
// GameBioplastState

GameBioplastState::GameBioplastState(const CoordXY &pStart, const CoordXY &pEnd) {
    m_p0 = pStart;
    m_pStart = pStart;
    m_pEnd = pEnd;
    m_bDestroyed = false;
    m_bCanBeRemoved = false;

    // distance
    double dx = m_pStart.x() - m_pEnd.x();
    double dy = m_pStart.y() - m_pEnd.y();
    m_nDistance = 0.0;
    m_nDistance = sqrt(dx * dx + dy * dy);

    // int nStep
    m_step = CoordXY(dx/10, dy/10); // TODO 10 must be size of bioplast
}

const CoordXY &GameBioplastState::getPosition() {
    return m_p0;
}

void GameBioplastState::move() {
    m_p0 += m_step;
    if (m_p0.y() > m_pEnd.y()) {
        m_bDestroyed = true;
    }
}

bool GameBioplastState::hasDestroyed() {
    return m_bDestroyed;
}

void GameBioplastState::destroy() {
    m_bDestroyed = true;
}

bool GameBioplastState::canBeRemoved() {
    return m_bCanBeRemoved;
}

void GameBioplastState::removeLater() {
    m_bCanBeRemoved = true;
}