#include <memory>

#include "shooting_strategy_logic.h"

ShootingStrategyLogic::ShootingStrategyLogic() {
    shootingMap[ShootingStrategies::None] = std::make_unique<NoneShootingStrategy>();
    shootingMap[ShootingStrategies::MovementShootingStrategy] =  std::make_unique<MovementShootingStrategy>();
    shootingMap[ShootingStrategies::RandomShootingStrategy] = std::make_unique<RandomShootingStrategy>();
    m_currentShootingStrategy = ShootingStrategies::None;
}

IShootingStrategy *ShootingStrategyLogic::getCurrentShootingStrategy() const {
    return shootingMap.at(m_currentShootingStrategy).get();
}

void ShootingStrategyLogic::setCurrentShootingStrategy(ShootingStrategies strategy) {
    if (shootingMap.find(strategy) != shootingMap.end()) {
        m_currentShootingStrategy = strategy;
    } else {
        std::cout << "Strategy not found" << std::endl;
    }
}

void ShootingStrategyLogic::onScoreChanged(int nScore) {
    if (nScore <= 0) {
        setCurrentShootingStrategy(ShootingStrategies::None);
    } else if (nScore > 0 && nScore < 5) { // range for MovementShootingStrategy
        setCurrentShootingStrategy(ShootingStrategies::MovementShootingStrategy);
    } else if (nScore >= 5) { // score for activating RandomShootingStrategy
        setCurrentShootingStrategy(ShootingStrategies::RandomShootingStrategy);
    }
}