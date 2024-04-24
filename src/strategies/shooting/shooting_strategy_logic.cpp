#include <memory>

#include "shooting_strategy_logic.h"

ShootingStrategyLogic::ShootingStrategyLogic() {
	std::unique_ptr<IShootingStrategy> randomShooting = std::make_unique<RandomShootingStrategy>();
	std::unique_ptr<IShootingStrategy> shootingUp = std::make_unique<ShootingUpStrategy>();
	shootingMap[ShootingStrategies::ShootingUpStrategy] = std::move(shootingUp);
	shootingMap[ShootingStrategies::RandomShootingStrategy] = std::move(randomShooting);
	m_currentShootingStrategy = ShootingStrategies::ShootingUpStrategy;
}

void ShootingStrategyLogic::resetCurrentShootingStrategy() {
	m_currentShootingStrategy = ShootingStrategies::ShootingUpStrategy;
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

void ShootingStrategyLogic::switchCurrentShootingStrategy(int nScore) {
	if (nScore == 0) {
		setCurrentShootingStrategy(ShootingStrategies::ShootingUpStrategy);
	} else if (nScore == 5) {
		setCurrentShootingStrategy(ShootingStrategies::RandomShootingStrategy);
	}
}