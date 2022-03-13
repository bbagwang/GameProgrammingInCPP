#pragma once

#include "Actor.h"

class CircleComponent;

class Laser : public Actor
{
public:
	Laser(Game* InGame);
	virtual ~Laser() = default;

	void UpdateActor(float DeltaTime) override;

private:
	CircleComponent* CircleComp;
	float DeathTimer;
};

