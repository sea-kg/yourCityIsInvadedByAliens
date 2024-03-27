#pragma once

#include "ishooting_strategy.h"


class RandomShootingStrategy : public IShootingStrategy {
    public:
        RandomShootingStrategy();

        virtual void shoot(const CoordXY &p0, std::vector<GameBioplastState *> &vBioplasts) override;

    private:
        CoordXY m_p0;
};

