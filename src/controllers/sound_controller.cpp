#include "sound_controller.h"
#include "ylog.h"
#include "ycore.h"
#include <algorithm>
#include <random>

SoundController::SoundController(
    const std::wstring &sResourceDir,
    GameState *pGameState
) {
    TAG = L"SoundController";
    m_sResourceDir = sResourceDir;
    m_pGameState = pGameState;

    // TODO redesign load from playlists.json
    m_vPlaylistEmbient.push_back(sResourceDir + L"/app/music/sea5kg - 00 AaaalientsBackOff.ogg");

    // fight list
    
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 07 CountdownOff (OST Game Your City Is Invaded by Aliens).ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 05 CrapRat (OST Game Your City Is Invaded By Aliens).ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 09 RetroMagicMome  (OST Game Your City Is Invaded by Aliens).ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 01 InvitedByAliens.ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 02 Diphdo.ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 03 SuchMyEnimies.ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 04 Tomorrow never been (OST Game Your City Is Invaded by Aliens).ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 06 Silly (Silent in the field) (OST Game Your City Is Invaded By Aliens).ogg");
    m_vPlaylistFight.push_back(sResourceDir + L"/app/music/sea5kg - 08 MobiMy (OST Game Your City Invaded By Aliens).ogg");

    // auto rng = std::default_random_engine {};
    // std::shuffle(std::begin(m_vPlaylistFight), std::end(m_vPlaylistFight), rng);

    // TODO redesign load from map.json
    m_vAttackedPaths.push_back(sResourceDir + L"/default/soundeffects/attacked1.wav");
    m_vAttackedPaths.push_back(sResourceDir + L"/default/soundeffects/attacked2.wav");
    m_vAttackedPaths.push_back(sResourceDir + L"/default/soundeffects/attacked3.wav");

    m_sTakeBerryPath = sResourceDir + L"/default/soundeffects/take_berry.wav";

}

SoundController::~SoundController() {

}

void SoundController::init() {
    if (m_pGameState->isPlayMusic()) {
        int nOpenAudioResult = Mix_OpenAudio(
            22050, // frequency
            AUDIO_S16SYS, // format
            2, // channels. Set to 2 for stereo, 1 for mono. This has nothing to do with mixing channels. 
            640 // chunksize
        );
        if (nOpenAudioResult == -1) {
            printf("Mix_OpenAudio: %s\n", Mix_GetError());
            exit(2);
        }
        Mix_VolumeMusic(25);

        for (int i = 0; i < m_vPlaylistFight.size(); i++) {
            std::wstring sPathToFile = m_vPlaylistFight[i];
            YLog::info(TAG, L"Trying load music from '" + sPathToFile + L"'");
            Mix_Music *pMusic = Mix_LoadMUS(YCore::ws2s(sPathToFile).c_str());
            if (!pMusic) {
                // YLog::info(TAG, L"Trying load music from '" + sPathToFile + L"'")
                std::cout << "ERROR: " << Mix_GetError() << std::endl;
                // this might be a critical error...
            } else {
                m_vPlaylistFightMusic.push_back(pMusic);
            }
        }
        m_nCurrentMusicFightTrack = m_vPlaylistFightMusic.size() > 0 ? 0 : -1;
    }

    for (int i = 0; i < m_vAttackedPaths.size(); i++) {
        YLog::info(TAG, L"Trying load soundeffects from '" + m_vAttackedPaths[i] + L"'");
        Mix_Chunk *pAttackedSoundEffect = Mix_LoadWAV(YCore::ws2s(m_vAttackedPaths[i]).c_str());
        if (!pAttackedSoundEffect) {
            // YLog::info(TAG, L"Trying load music from '" + sPathToFile + L"'")
            std::cout << "ERROR: " << Mix_GetError() << std::endl;
            // this might be a critical error...
        } else {
            m_vAttackedSoundEffect.push_back(pAttackedSoundEffect);
        }
    }

    

    YLog::info(TAG, L"Trying load soundeffects from '" + m_sTakeBerryPath + L"'");
    m_pTakeBerrySoundEffect = Mix_LoadWAV(YCore::ws2s(m_sTakeBerryPath).c_str());
    if (!m_pTakeBerrySoundEffect) {
        // YLog::info(TAG, L"Trying load music from '" + sPathToFile + L"'")
        std::cout << "ERROR: " << Mix_GetError() << std::endl;
        // this might be a critical error...
    }

    Mix_AllocateChannels(100);
}

void SoundController::update() {
    if (m_pGameState->isPlayMusic()) {
        if (m_vPlaylistFightMusic.size() == 0) { // not load music
            return;
        }

        // printf("Mix_PlayingMusic: %d\n", Mix_PlayingMusic());
        // printf("Mix_PausedMusic: %d\n", Mix_PausedMusic());
        if (Mix_PlayingMusic() == 0) { // no playing
            Mix_PlayMusic(
                m_vPlaylistFightMusic[m_nCurrentMusicFightTrack],
                1 // plays the music zero times... (-1 forever loop)
            );
            m_nCurrentMusicFightTrack++;
            m_nCurrentMusicFightTrack = m_nCurrentMusicFightTrack % m_vPlaylistFightMusic.size();
        } else {
            // nothing
        }
    }


    // int Mix_HaltChannel(int channel); - stop playin channel
    // TODO implement pause
    // if( Mix_PausedMusic() == 1 )
    // {
    //     //Продолжить играть
    //     Mix_ResumeMusic();
    // }
    // //Если музыка играет
    // else
    // {
    //     //Приостановить проигрывание
    //     Mix_PauseMusic();
    // }
    // Mix_HaltMusic();

    // TODO Mix_FadeOutMusic
}


void SoundController::playAttacked() {
    Mix_Chunk *pAttackedSoundEffect = m_vAttackedSoundEffect[std::rand() % m_vAttackedSoundEffect.size()];
    Mix_PlayChannel(2, pAttackedSoundEffect, 0);
}

void SoundController::playTakeBerry() {
    Mix_PlayChannel(3, m_pTakeBerrySoundEffect, 0);
}

void SoundController::stopTakeBerry() {
    Mix_HaltChannel(3);
}
