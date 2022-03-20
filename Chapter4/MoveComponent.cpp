#include "MoveComponent.h"

#include "Math.h"

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
		float rot = Owner->GetRotation();
		rot += AngularSpeed * DeltaTime;
		Owner->SetRotation(rot);
	}

	if (!Math::NearZero(ForwardSpeed))
	{
		Vector2 pos = Owner->GetPosition();
		pos += Owner->GetForward() * ForwardSpeed * DeltaTime;
		Owner->SetPosition(pos);
	}
}
