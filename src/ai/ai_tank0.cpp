
#include "ai_tank0.h"

AiTank0::AiTank0() {

}

void AiTank0::makeStep(GameTank0State &tank0State) {
    if (!tank0State.hasRocket()) {
        tank0State.rechargeRocket();
        return;
    }
    int nRandom = std::rand() % 4;
    if (nRandom == 0) {
        tank0State.turnLeft();
    } else if (nRandom == 1) {
        tank0State.turnRight();
    } else if (nRandom == 2) {
        tank0State.move();
    } else if (nRandom == 3) {
        // tank0State.shotRocket();
    }
}