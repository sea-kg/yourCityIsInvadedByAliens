#pragma once
#include "game_bioplast_state.h"
#include "move_object_direction.h"

class IShootingStrategy {
    public:
        virtual void shoot(const YPos &p0, MoveObjectDirection direction) = 0;
        virtual GameBioplastState *popBioplast() = 0;
};