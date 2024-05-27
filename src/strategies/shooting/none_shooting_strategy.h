#pragma once
#include "ishooting_strategy.h"
#include <memory>

class NoneShootingStrategy : public IShootingStrategy {
    public:
        virtual void shoot(const YPos& p0) override;
        virtual GameBioplastState* popBioplast() override;
    private:
        std::vector<GameBioplastState*> m_vBioplasts;
};

