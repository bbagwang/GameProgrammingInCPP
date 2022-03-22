#include "ActorComponent.h"

ActorComponent::ActorComponent(Actor* InOwner, int InUpdateOrder)
	: Owner(InOwner)
	, UpdateOrder(InUpdateOrder)
{
	if (Owner)
	{
		//소유자의 컴포넌트로 등록한다
		Owner->AddComponent(this);
	}
}

ActorComponent::~ActorComponent()
{
	if (Owner)
	{
		Owner->RemoveComponent(this);
	}
}

void ActorComponent::Update(float DeltaTime)
{
}
