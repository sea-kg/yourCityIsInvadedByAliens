#pragma once
#include <vector>
#include "coordxy.h"
#include "move_object_direction.h"
#include "json.hpp"

class GameBuilding {
    public:
        GameBuilding(nlohmann::json &jsonData);
        const std::string &getName();
        const std::vector<CoordXY> &getPoints();

    private:
        std::string m_sName;
        std::vector<CoordXY> m_vPoints;
};