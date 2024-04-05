#pragma once
#include "ishooting_strategy.h"

class ShootingUpStrategy : public IShootingStrategy {
public:
	virtual void shoot(const CoordXY& p0) override;
	virtual GameBioplastState *popBioplast() override;

private:
	std::vector<GameBioplastState *> m_vBioplasts;
};