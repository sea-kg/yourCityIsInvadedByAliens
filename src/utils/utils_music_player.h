#pragma once
#include <string>
#include "render.h"

class UtilsMusicPlayer {
    public:
        UtilsMusicPlayer(
            const std::string &sResourceDir,
            GameState *pGameState
        );
        ~UtilsMusicPlayer();
        void init();


    private:
        std::string m_sResourceDir;
        GameState *m_pGameState;
};