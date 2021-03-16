#include "game_building.h"
#include <SDL.h>


// ---------------------------------------------------------------------
// GameBuilding

GameBuilding::GameBuilding(nlohmann::json &jsonData) {
    m_sName = jsonData["name"];
    int nPoints = jsonData["points"];
    for (int i = 0; i < nPoints; i++) {
        int nX = jsonData["x" + std::to_string(i)];
        int nY = jsonData["y" + std::to_string(i)];
        m_vPoints.push_back(CoordXY(nX,nY));
    }
}

const std::string &GameBuilding::getName() {
    return m_sName;
}

const std::vector<CoordXY> &GameBuilding::getPoints() {
    return m_vPoints;
}
