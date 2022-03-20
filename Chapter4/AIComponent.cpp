#include "AIComponent.h"

#include "AIState.h"

AIComponent::AIComponent(Actor* InOwner, int InUpdateOrder)
	: ActorComponent(InOwner, InUpdateOrder)
{

}

void AIComponent::Update(float DeltaTime)
{
	if (!CurrentState)
		return;

	CurrentState->Update(DeltaTime);
}

void AIComponent::ChangeState(const std::string& StateName)
{
	//먼저 현재 상태를 빠져 나온다.
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	auto Iter = StateMap.find(StateName);
	if (Iter != StateMap.end())
	{
		CurrentState = Iter->second;
		//새로운 상태로 진입한다.
		CurrentState->OnEnter();
	}
	else
	{
		SDL_Log("AIState [%s] 못찾음.", StateName.c_str());
		CurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* State)
{
	if (!State)
		return;

	StateMap.emplace(State->GetName(), State);
}
