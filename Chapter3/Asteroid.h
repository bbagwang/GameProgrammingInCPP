#pragma once

#include "Actor.h"

class CircleComponent;

class Asteroid : public Actor
{
public:
	Asteroid(Game* InGame);
	virtual ~Asteroid() override;

	inline CircleComponent* GetCircle() { return CircleComp; }

private:
	CircleComponent* CircleComp;
};

