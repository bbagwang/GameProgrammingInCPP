#pragma once

#include "ActorComponent.h"

class MoveComponent : public ActorComponent
{
public:
	//먼저 업데이트되도록 갱신 순서를 낮춤
	MoveComponent(Actor* InOwner, int InUpdateOrder = 10);
	virtual ~MoveComponent() = default;

	virtual void Update(float DeltaTime) override;

	float GetAngularSpeed() const { return AngularSpeed; }
	float GetForwardSpeed() const { return ForwardSpeed; }
	void SetAngularSpeed(float Speed) { AngularSpeed = Speed; }
	void SetForwardSpeed(float Speed) { ForwardSpeed = Speed; }

private:
	//회전을 제어(초당 라디안)
	float AngularSpeed;
	//전방 이동을 제어(초당 단위)
	float ForwardSpeed;
};
