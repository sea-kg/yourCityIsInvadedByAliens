#pragma once
#include "game_tank0_state.h"
#include "ai_object.h"
#include "map_yservice.h"

class AiTank0 : public AiObject {
    public:
        AiTank0(GameTank0State *pTank0State);
        void makeStep();
    private:
        GameTank0State *m_pTank0State;
        MapYService *m_pMap;
};