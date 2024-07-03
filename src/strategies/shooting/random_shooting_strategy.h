#pragma once
#include "ishooting_strategy.h"

class RandomShootingStrategy : public IShootingStrategy {
    public:
        virtual void shoot(const YPos& p0, MoveObjectDirection direction) override;
        virtual GameBioplastState *popBioplast() override;

    private:
        std::vector<GameBioplastState *> m_vBioplasts;
};