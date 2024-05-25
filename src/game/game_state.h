#pragma once
#include <vector>
#include <iostream>
#include "game_building.h"
#include "move_object_direction.h"
#include "game_alien_ship_state.h"

long getCurrentTimeInMilliseconds();

class GameState {

    public:
        GameState(int windowWidth, int windowHeight);
        void init();
        void updateElapsedTime();
        void addBuilding(GameBuilding *);
        long getElapsedTime() const;
        const YPos &getCoordLeftTop() const;
        void setCoordLeftTop(const YPos &);

        bool isChangedWindowSize() const;
        int getWindowWidth() const;
        int getWindowHeight() const;
        const YRect &getWindowRect() const;
        void updateWindowSize(int w, int h);

        void setMouseCaptured(bool bMouseCaptured);
        bool isMouseCaptured() const;

        void setPlayMusic(bool bPlayMusic);
        void togglePlayMusic();
        bool isPlayMusic() const;

        bool isPauseGame() const;
        void setPauseGame(bool bPauseGame);

        void setMinPoint(const YPos &p);
        const YPos &getMinPoint();
        void setMaxPoint(const YPos &p);
        const YPos &getMaxPoint();

        void setShowLoader(bool bShowLoader);
        bool isShowLoader() const;

        GameAlienShipState *getAlienShipState();
        void updatePlayerStartPosition(const YPos &playerStartPosition);
        const YPos &getPlayerPosition() const;

    private:
        bool m_bMouseCaptured;
        bool m_bPlayMusic;
        bool m_bPauseGame;
        bool m_bShowLoader;
        long m_nElapsedTime;
        long m_nStartTime;
        YPos m_coordLeftTop;
        YPos m_maxPoint;
        YPos m_minPoint;

        bool m_bIsChangedWindowSize;
        int m_nWindowWidth;
        int m_nWindowHeight;
        YRect m_windowRect;
        std::vector<GameBuilding *> m_vBuildings;

        GameAlienShipState *m_pAlienShipState;
        YPos m_playerStartPosition;
};
