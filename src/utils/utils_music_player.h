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
        void update();

    private:
        std::string m_sResourceDir;
        GameState *m_pGameState;
        std::vector<std::string> m_vPlaylistFight;
        std::vector<Mix_Music *> m_vPlaylistFightMusic;
        int m_nCurrentMusicFightTrack = 0;

        std::vector<std::string> m_vPlaylistEmbient;
        
};