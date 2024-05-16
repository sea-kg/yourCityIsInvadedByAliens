#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>

#include "ishooting_strategy.h"
#include "shooting_up_strategy.h"
#include "random_shooting_strategy.h"
#include "none_shooting_strategy.h"

enum class ShootingStrategies {
    None,
    ShootingUpStrategy,
    RandomShootingStrategy
};

class ShootingStrategyLogic {
    public:
        ShootingStrategyLogic();
        IShootingStrategy *getCurrentShootingStrategy() const;
        void onScoreChanged(int nScore);

    private:
        void setCurrentShootingStrategy(ShootingStrategies strategy);
        std::unordered_map<ShootingStrategies, std::unique_ptr<IShootingStrategy>> shootingMap;
        ShootingStrategies m_currentShootingStrategy;
};

