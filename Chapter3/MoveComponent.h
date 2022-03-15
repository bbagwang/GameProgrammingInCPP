#pragma once

#include "ActorComponent.h"

class MoveComponent : public ActorComponent
{
public:
	//���� ������Ʈ�ǵ��� ���� ������ ����
	MoveComponent(Actor* InOwner, int InUpdateOrder = 10);
	virtual ~MoveComponent() = default;

	virtual void Update(float DeltaTime) override;

	void AddForce(const Vector2& InForce);
	void ClearForce() { AccumulatedForce.Set(0.f, 0.f); }
	
	float GetAngularSpeed() const { return AngularSpeed; }
	float GetForwardSpeed() const { return ForwardSpeed; }
	void SetAngularSpeed(float Speed) { AngularSpeed = Speed; }
	void SetForwardSpeed(float Speed) { ForwardSpeed = Speed; }

private:
	//ȸ���� ����(�ʴ� ����)
	float AngularSpeed;
	//���� �̵��� ����(�ʴ� ����)
	float ForwardSpeed;

	//������ ��
	Vector2 AccumulatedForce;

	//����
	float Mass;
	//���ӵ�
	float Acceleration;
	//�ӵ�
	Vector2 Velocity;
};
