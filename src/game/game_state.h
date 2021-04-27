#pragma once
#include <vector>
#include "coordxy.h"
#include <iostream>
#include "game_building.h"
#include "move_object_direction.h"

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

        bool isChangedWindowSize() const;
        const int getWindowWidth() const;
        const int getWindowHeight() const;
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

    private:
        bool m_bMouseCaptured;
        bool m_bPlayMusic;
        long m_nElapsedTime;
        long m_nStartTime;
        CoordXY m_coordLeftTop;
        CoordXY m_maxPoint;
        CoordXY m_minPoint;
        bool m_bIsChangedWindowSize;
        int m_nWindowWidth;
        int m_nWindowHeight;
        std::vector<GameBuilding *> m_vBuildings;
};
