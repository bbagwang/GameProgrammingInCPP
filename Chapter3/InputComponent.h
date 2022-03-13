#pragma once

#include "MoveComponent.h"

class InputComponent : public MoveComponent
{
public:
	InputComponent(Actor* InOwner);
	virtual ~InputComponent() = default;

	virtual void ProcessInput(const uint8_t* KeyState) override;

	inline float GetMaxForwardSpeed() const { return MaxForwardSpeed; }
	inline float GetMaxAngularSpeed() const { return MaxAngularSpeed; }
	inline int GetForwardKey() const { return ForwardKey; }
	inline int GetBackwardKey() const { return BackwardKey; }
	inline int GetClockwiseKey() const { return ClockwiseKey; }
	inline int GetCounterClockwiseKey() const { return CounterClockwiseKey; }

	inline void SetMaxForwardSpeed(float Speed) { MaxForwardSpeed = Speed; }
	inline void SetMaxAngularSpeed(float Speed) { MaxAngularSpeed = Speed; }
	inline void SetForwardKey(int Key) { ForwardKey = Key; }
	inline void SetBackwardKey(int Key) { BackwardKey = Key; }
	inline void SetClockwiseKey(int Key) { ClockwiseKey = Key; }
	inline void SetCounterClockwiseKey(int Key) { CounterClockwiseKey = Key; }

private:
	//최대 전방 속도 / 최대 각속도
	float MaxForwardSpeed;
	float MaxAngularSpeed;

	//전진 및 후진을 위한 키
	int ForwardKey;
	int BackwardKey;

	//각운동을 위한 키
	int ClockwiseKey;
	int CounterClockwiseKey;
};

