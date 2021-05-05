#include "utils_music_player.h"
#include <chrono>
#include <thread>

// TODO redesign to load filelist from res
static const char *MY_COOL_MP3 = "res/app/music/sea5kg - 01 InvitedByAliens.ogg";

UtilsMusicPlayer::UtilsMusicPlayer(
    const std::string &sResourceDir,
    GameState *pGameState
) {
    m_sResourceDir = sResourceDir;
    m_pGameState = pGameState;
}

UtilsMusicPlayer::~UtilsMusicPlayer() {

}

void UtilsMusicPlayer::init() {
    if (m_pGameState->isPlayMusic()) {
        Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
        Mix_Music *music = Mix_LoadMUS(MY_COOL_MP3);
        Mix_PlayMusic(music, 1);
    }
}