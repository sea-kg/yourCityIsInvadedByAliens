#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>

#include "ishooting_strategy.h"
#include "shooting_up_strategy.h"
#include "random_shooting_strategy.h"

enum class ShootingStrategies {
	ShootingUpStrategy,
	RandomShootingStrategy
};

class ShootingStrategyLogic {
public:
	ShootingStrategyLogic();
	IShootingStrategy *getCurrentShootingStrategy() const;
	void setCurrentShootingStrategy(ShootingStrategies strategy);
	void resetCurrentShootingStrategy();
	void switchCurrentShootingStrategy(int nScore);

private:
	std::unordered_map<ShootingStrategies, std::unique_ptr<IShootingStrategy>> shootingMap;
	ShootingStrategies m_currentShootingStrategy;
};

