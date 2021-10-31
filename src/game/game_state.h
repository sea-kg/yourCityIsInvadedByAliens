#pragma once
#include <vector>
#include "coordxy.h"
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
        const CoordXY &getCoordLeftTop() const;
        void setCoordLeftTop(const CoordXY &);
        const YPos &getYPosLeftTop() const;

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

        void setMinPoint(const CoordXY &p);
        const CoordXY &getMinPoint();
        void setMaxPoint(const CoordXY &p);
        const CoordXY &getMaxPoint();

        void setShowLoader(bool bShowLoader);
        bool isShowLoader() const;

        GameAlienShipState *getAlienShipState();
        void updatePlayerStartPosition(const CoordXY &playerStartPosition);

    private:
        bool m_bMouseCaptured;
        bool m_bPlayMusic;
        bool m_bShowLoader;
        long m_nElapsedTime;
        long m_nStartTime;
        YPos m_posLeftTop;
        CoordXY m_coordLeftTop;
        CoordXY m_maxPoint;
        CoordXY m_minPoint;
        
        bool m_bIsChangedWindowSize;
        int m_nWindowWidth;
        int m_nWindowHeight;
        YRect m_windowRect;
        std::vector<GameBuilding *> m_vBuildings;

        GameAlienShipState *m_pAlienShipState;
        CoordXY m_playerStartPosition;
};
