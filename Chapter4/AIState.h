#pragma once

class AIComponent;

class AIState
{
public:
	AIState(AIComponent* InOwner) : Owner(InOwner) {}
	virtual ~AIState() = default;

	//�� ������ ��ü���� �ൿ
	virtual void Update(float DeltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	//������ �̸� ���
	virtual const char* GetName() const = 0;

protected:
	AIComponent* Owner;
};
