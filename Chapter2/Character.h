#pragma once

#include "Actor.h"

class AnimSpriteComponent;

enum AnimState
{
	None = -1,
	Idle = 0,
	Move = 1,
	Jump = 2,
	Punch = 3
};

class Character : public Actor
{
public:
	Character(Game* InGame);
	virtual ~Character() = default;

	virtual void UpdateActor(float DeltaTime) override;
	void ProcessKeyboard(const uint8_t* State);

	void SetAnimState(AnimState NewState);
	AnimState GetAnimState() const;

	inline bool GetJumpingState() const { return bJumpingState; }

private:
	AnimSpriteComponent* ASC;

	Vector2 Velocity;

	float JumpScale = 600.f;
	float GravityScale = 980.f;

	bool bJumpingState = false;
};

