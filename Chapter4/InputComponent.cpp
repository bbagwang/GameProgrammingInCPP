#include "InputComponent.h"

InputComponent::InputComponent(Actor* InOwner)
	: MoveComponent(InOwner)
	, MaxForwardSpeed(0.f)
	, MaxAngularSpeed(0.f)
	, ForwardKey(0)
	, BackwardKey(0)
	, ClockwiseKey(0)
	, CounterClockwiseKey(0)
{
}

void InputComponent::ProcessInput(const uint8_t* KeyState)
{
	if (!Owner)
		return;

	//MoveComponent 를 위한 전방 속도 계산
	float ForwardSpeed = 0.f;
	Vector2 NewForce;
	if (KeyState[ForwardKey])
	{
		ForwardSpeed += MaxForwardSpeed * 10.f;
	}
	if (KeyState[BackwardKey])
	{
		ForwardSpeed -= MaxForwardSpeed * 10.f;
	}

	SetForwardSpeed(ForwardSpeed);

	//MoveComponent 를 위한 각속도 계산
	float AngularSpeed = 0.f;
	if (KeyState[ClockwiseKey])
	{
		AngularSpeed += MaxAngularSpeed;
	}
	if (KeyState[CounterClockwiseKey])
	{
		AngularSpeed -= MaxAngularSpeed;
	}

	SetAngularSpeed(AngularSpeed);
}
