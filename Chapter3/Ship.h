#pragma once

#include "Actor.h"

#include "ShipState.h"

class CircleComponent;
class SpriteComponent;
class InputComponent;

class Ship : public Actor
{
public:
	Ship(Game* InGame);
	virtual ~Ship() = default;

	virtual void UpdateActor(float DeltaTime) override;
	virtual void ActorInput(const uint8_t* KeyState) override;

	inline CircleComponent* GetCircle() const { return CircleComp; }

	inline ShipState GetShipState() const { return ShipStateValue; }

protected:
	void OnCollide(Actor* CollideActor);

	void SetCrashedState(bool bNewState);

private:
	float LaserCooldown;
	
	float TotalInvincibleTime;
	float CurrentInvincibleTime;

	ShipState ShipStateValue;

	SpriteComponent* SpriteComp;
	CircleComponent* CircleComp;
	InputComponent* InputComp;
};

