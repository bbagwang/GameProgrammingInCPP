#pragma once

#include "AIState.h"

class AIPatrol : public AIState
{
public:
	AIPatrol(AIComponent* InOwner);
	virtual ~AIPatrol() = default;

	//�� ������ ��ü���� �ൿ
	virtual void Update(float DeltaTime) override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	//������ �̸� ���
	virtual const char* GetName() const override { return "Patrol"; }
};

