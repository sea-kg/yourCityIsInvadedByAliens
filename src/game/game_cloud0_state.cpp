#include "game_cloud0_state.h"

// ---------------------------------------------------------------------
// GameCloud0State

GameCloud0State::GameCloud0State(const CoordXY &p0) {
    m_p0 = p0;
    
    m_nMoves = 0;
    
    randomDiff();
}

const CoordXY &GameCloud0State::getPosition() {
    return m_p0;
}

void GameCloud0State::move() {
    m_nMoves++;
    if (m_nMoves > m_nMaxMoves) {
        randomDiff();
        m_nMoves = 0;
    }
    m_p0 += m_pDiff;
}

void GameCloud0State::randomDiff() {
    int nStep = 3;
    int nXpos = std::rand() % (nStep*2 + 1);
    nXpos -= nStep;
    int nYpos = std::rand() % (nStep*2 + 1);
    nYpos -= nStep;
    m_pDiff = CoordXY(nXpos, nYpos);
    m_nMaxMoves = 20 + std::rand() % 100;
}