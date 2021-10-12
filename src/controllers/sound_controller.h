#pragma once
#include <string>
#include "render.h"

class SoundController {
    public:
        SoundController(
            const std::string &sResourceDir,
            GameState *pGameState
        );
        ~SoundController();
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