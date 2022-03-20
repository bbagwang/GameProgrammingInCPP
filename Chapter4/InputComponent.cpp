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

	//MoveComponent �� ���� ���� �ӵ� ���
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
