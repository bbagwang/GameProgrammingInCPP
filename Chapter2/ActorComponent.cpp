#include "ActorComponent.h"

ActorComponent::ActorComponent(Actor* InOwner, int InUpdateOrder)
	: Owner(InOwner)
	, UpdateOrder(InUpdateOrder)
{
	if (Owner)
	{
		//�������� ������Ʈ�� ����Ѵ�
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
