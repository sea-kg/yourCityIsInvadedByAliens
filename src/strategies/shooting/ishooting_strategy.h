#pragma once
#include "game_bioplast_state.h"

class IShootingStrategy {
    public:
        virtual void shoot(const CoordXY &p0) = 0;
        virtual GameBioplastState *popBioplast() = 0;
};