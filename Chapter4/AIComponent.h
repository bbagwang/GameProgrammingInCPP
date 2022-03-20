#pragma once

#include "ActorComponent.h"

class AIState;

class AIComponent : public ActorComponent
{
public:
	AIComponent(Actor* InOwner, int InUpdateOrder = 100);
	virtual ~AIComponent() = default;

	virtual void Update(float DeltaTime) override;

	//AI ���¸� �ٲ۴�.
	void ChangeState(const std::string& StateName);
	
	//�� ���¸� �ʿ� �߰��Ѵ�
	void RegisterState(AIState* State);

protected:
	//������ �̸��� AIState �ν��Ͻ��� �����Ѵ�
	std::unordered_map<std::string, AIState*> StateMap;
	
	//AI ���� ����
	AIState* CurrentState;
};
