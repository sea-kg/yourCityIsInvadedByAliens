
#include "ai_tank0.h"

AiTank0::AiTank0(GameTank0State *pTank0State) : AiObject() {
    m_pTank0State = pTank0State;
}

void AiTank0::makeStep() {
    if (!m_pTank0State->hasRocket()) {
        m_pTank0State->rechargeRocket();
        return;
    }
    int nRandom = std::rand() % 4;
    if (nRandom == 0) {
        m_pTank0State->turnLeft();
    } else if (nRandom == 1) {
        m_pTank0State->turnRight();
    } else if (nRandom == 2) {
        m_pTank0State->move();
    } else if (nRandom == 3) {
        m_pTank0State->shotRocket();
    }
}