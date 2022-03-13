#pragma once

#include "ActorComponent.h"

class MoveComponent : public ActorComponent
{
public:
	//���� ������Ʈ�ǵ��� ���� ������ ����
	MoveComponent(Actor* InOwner, int InUpdateOrder = 10);
	virtual ~MoveComponent() = default;

	virtual void Update(float DeltaTime) override;

	float GetAngularSpeed() const { return AngularSpeed; }
	float GetForwardSpeed() const { return ForwardSpeed; }
	void SetAngularSpeed(float Speed) { AngularSpeed = Speed; }
	void SetForwardSpeed(float Speed) { ForwardSpeed = Speed; }

private:
	//ȸ���� ����(�ʴ� ����)
	float AngularSpeed;
	//���� �̵��� ����(�ʴ� ����)
	float ForwardSpeed;
};
