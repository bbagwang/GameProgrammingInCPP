#pragma once

#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(Game* InGame);
	virtual ~Ship() = default;

	void UpdateActor(float DeltaTime) override;
	void ProcessKeyboard(const uint8_t* State);
	
	inline float GetRightSpeed() const { return RightSpeed; }
	inline float GetDownSpeed() const { return DownSpeed; }

private:
	float RightSpeed;
	float DownSpeed;
};

