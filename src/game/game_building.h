#pragma once

#include <vector>
#include "move_object_direction.h"
#include "ypos.h"
#include "yjson.h"

class GameBuilding {
    public:
        GameBuilding(const YJsonObject &jsonData);
        const std::wstring &getName();
        const std::vector<YPos> &getPoints();

    private:
        std::wstring m_sName;
        std::vector<YPos> m_vPoints;
};