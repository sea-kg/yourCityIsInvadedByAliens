#include "random_shooting_strategy.h"
#include "game_alien_ship_state.h"

RandomShootingStrategy::RandomShootingStrategy() {};

void RandomShootingStrategy::shoot(const CoordXY &p0, std::vector<GameBioplastState *> &vBioplasts) {
    int nX = std::rand() % 500 - 250;
    int nY = std::rand() % 500 - 250;
    vBioplasts.push_back(new GameBioplastState(p0, p0 + CoordXY(nX, nY)));
}
