#pragma once

#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(Game* InGame);
	virtual ~Ship() = default;

	virtual void UpdateActor(float DeltaTime) override;
	virtual void ActorInput(const uint8_t* KeyState) override;

private:
	float LaserCooldown;
};

