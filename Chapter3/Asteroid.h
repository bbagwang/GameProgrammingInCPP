#pragma once

#include "Actor.h"

class CircleComponent;

class Asteroid : public Actor
{
public:
	Asteroid(Game* InGame);
	virtual ~Asteroid() override;

	virtual void UpdateActor(float DeltaTime) override;
	
	inline CircleComponent* GetCircle() const { return CircleComp; }

private:
	CircleComponent* CircleComp;
};

