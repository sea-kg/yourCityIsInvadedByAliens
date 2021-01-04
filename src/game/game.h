#pragma once
#include <vector>
#include "json.hpp"

class CoordXY {
    public:
        CoordXY();
        CoordXY(int x, int y);
        int x() const;
        int y() const;
        void update(int x, int y);

        CoordXY& operator+=(const CoordXY& other);
        CoordXY& operator-=(const CoordXY& other);
        CoordXY operator+(const CoordXY& other) const;
    private:
        int m_nX, m_nY;
};

class GameBuilding {
    public:
        GameBuilding(nlohmann::json &jsonData);
        const std::string &getName();
        const std::vector<CoordXY> &getPoints();

    private:
        std::string m_sName;
        std::vector<CoordXY> m_vPoints;
};

class GameState {

    public:
        GameState(int windowWidth, int windowHeight);
        void init();
        void updateElapsedTime();
        void addBuilding(GameBuilding *);
        long getElapsedTime() const;
        const CoordXY &getCoordLeftTop() const;
        void incrementCoordLeftTopX(int nX);
        void incrementCoordLeftTopY(int nY);
        
        const int windowWidth() const;
        const int windowHeight() const;

    private:
        long m_nElapsedTime;
        long m_nStartTime;
        CoordXY m_coordLeftTop;
        int m_nWindowWidth;
        int m_nWindowHeight;
        std::vector<GameBuilding *> m_vBuildings;
};