#pragma once
#include <string>
#include "render.h"

class SoundController {
    public:
        SoundController(
            const std::wstring &sResourceDir,
            GameState *pGameState
        );
        ~SoundController();
        void init();
        void update();

        void playAttacked();
        void playTakeBerry();
        void stopTakeBerry();

    private:
        std::wstring TAG;

        std::wstring m_sResourceDir;
        GameState *m_pGameState;
        std::vector<std::wstring> m_vPlaylistFight;
        std::vector<Mix_Music *> m_vPlaylistFightMusic;
        int m_nCurrentMusicFightTrack = 0;

        std::vector<std::wstring> m_vPlaylistEmbient;
        
        std::vector<std::wstring> m_vAttackedPaths;
        std::vector<Mix_Chunk *> m_vAttackedSoundEffect;
        
        std::wstring m_sTakeBerryPath;
        Mix_Chunk *m_pTakeBerrySoundEffect;
};