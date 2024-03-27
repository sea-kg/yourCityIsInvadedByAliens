#pragma once

#include "game_bioplast_state.h"

class IShootingStrategy {
    public:
        virtual void shoot(const CoordXY &p0, std::vector<GameBioplastState *> &vBioplasts) = 0;

	private:
        // virtual float shootingVelocity() = 0;
        // virtual void shootingDirection() = 0;
        // virtual int shootingIntencity() = 0;
};