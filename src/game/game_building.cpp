#include "game_building.h"
#include <SDL.h>


// ---------------------------------------------------------------------
// GameBuilding

GameBuilding::GameBuilding(const YJsonObject &jsonData) {
    m_sName = jsonData[L"name"].getString();
    int nPoints = jsonData[L"points"].getNumber();
    for (int i = 0; i < nPoints; i++) {
        int nX = jsonData[L"x" + std::to_wstring(i)].getNumber();
        int nY = jsonData[L"y" + std::to_wstring(i)].getNumber();
        m_vPoints.push_back(YPos(nX,nY));
    }
}

const std::wstring &GameBuilding::getName() {
    return m_sName;
}

const std::vector<YPos> &GameBuilding::getPoints() {
    return m_vPoints;
}
