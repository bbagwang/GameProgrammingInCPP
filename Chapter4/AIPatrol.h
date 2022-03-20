#pragma once

#include "AIState.h"

class AIPatrol : public AIState
{
public:
	AIPatrol(AIComponent* InOwner);
	virtual ~AIPatrol() = default;

	//각 상태의 구체적인 행동
	virtual void Update(float DeltaTime) override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	//상태의 이름 얻기
	virtual const char* GetName() const override { return "Patrol"; }
};

