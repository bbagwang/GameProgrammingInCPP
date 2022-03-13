#include "MoveComponent.h"

MoveComponent::MoveComponent(Actor* InOwner, int InUpdateOrder)
	: ActorComponent(InOwner, InUpdateOrder)
	, AngularSpeed(0.f)
	, ForwardSpeed(0.f)
{
}

void MoveComponent::Update(float DeltaTime)
{
	if (!Math::NearZero(AngularSpeed))
	{
		float NewRotation = Owner->GetRotation();
		NewRotation += AngularSpeed * DeltaTime;
		Owner->SetRotation(NewRotation);
	}

	if (!Math::NearZero(ForwardSpeed))
	{
		Vector2 NewPosition = Owner->GetPosition();
		NewPosition += Owner->GetForward() * ForwardSpeed * DeltaTime;
		Owner->SetPosition(NewPosition);
	}
}
