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
	//���� ���� ���¸� ���� ���´�.
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	auto Iter = StateMap.find(StateName);
	if (Iter != StateMap.end())
	{
		CurrentState = Iter->second;
		//���ο� ���·� �����Ѵ�.
		CurrentState->OnEnter();
	}
	else
	{
		SDL_Log("AIState [%s] ��ã��.", StateName.c_str());
		CurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* State)
{
	if (!State)
		return;

	StateMap.emplace(State->GetName(), State);
}
