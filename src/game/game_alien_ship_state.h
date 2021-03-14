#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "game_bioplast_state.h"
#include "json.hpp"

class GameAlienShipState {
    public:
        GameAlienShipState(const CoordXY &p0);
        const CoordXY &getPosition();
        void shot();
        GameBioplastState *popRocket();

    private:
        CoordXY m_p0;
        std::vector<GameBioplastState *> m_vBioplasts;
};
