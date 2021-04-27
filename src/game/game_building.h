#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "yjson.h"

class GameBuilding {
    public:
        GameBuilding(const YJsonObject &jsonData);
        const std::string &getName();
        const std::vector<CoordXY> &getPoints();

    private:
        std::string m_sName;
        std::vector<CoordXY> m_vPoints;
};