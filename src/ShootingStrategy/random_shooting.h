#pragma once
#include "shooting_strategy.h"
#include <vector>
#include "game_bioplast_state.h"
class RandomShooting : public ShootingStrategy {
public:
	RandomShooting();
	void shoot() override;
	GameBioplastState* popBioplast();

private:
	std::vector<GameBioplastState*> m_vBioplasts;
	CoordXY m_p0;
};

