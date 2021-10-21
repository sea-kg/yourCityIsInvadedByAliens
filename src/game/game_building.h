#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "yjson.h"

class GameBuilding {
    public:
        GameBuilding(const YJsonObject &jsonData);
        const std::wstring &getName();
        const std::vector<CoordXY> &getPoints();

    private:
        std::wstring m_sName;
        std::vector<CoordXY> m_vPoints;
};