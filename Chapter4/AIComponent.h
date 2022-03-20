#pragma once

#include "ActorComponent.h"

class AIState;

class AIComponent : public ActorComponent
{
public:
	AIComponent(Actor* InOwner, int InUpdateOrder = 100);
	virtual ~AIComponent() = default;

	virtual void Update(float DeltaTime) override;

	//AI 상태를 바꾼다.
	void ChangeState(const std::string& StateName);
	
	//새 상태를 맵에 추가한다
	void RegisterState(AIState* State);

protected:
	//상태의 이름과 AIState 인스턴스를 매핑한다
	std::unordered_map<std::string, AIState*> StateMap;
	
	//AI 현재 상태
	AIState* CurrentState;
};
