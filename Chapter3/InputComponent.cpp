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
	//MoveComponent �� ���� ���� �ӵ� ���
	float ForwardSpeed = 0.f;
	if (KeyState[ForwardKey])
	{
		ForwardSpeed += MaxForwardSpeed;
	}
	if (KeyState[BackwardKey])
	{
		ForwardSpeed -= MaxForwardSpeed;
	}

	SetForwardSpeed(ForwardSpeed);

	//MoveComponent �� ���� ���ӵ� ���
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
