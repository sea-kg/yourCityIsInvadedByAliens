#pragma once
class ShootingStrategy {
public:
	virtual void shoot() = 0;

private:
	virtual float shootingVelocity() = 0;
	virtual void shootingDirection() = 0;
	virtual int shootingIntencity() = 0;
};