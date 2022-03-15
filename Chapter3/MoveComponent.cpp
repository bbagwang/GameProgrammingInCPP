#include "MoveComponent.h"

#include "Math.h"

MoveComponent::MoveComponent(Actor* InOwner, int InUpdateOrder)
	: ActorComponent(InOwner, InUpdateOrder)
	, AngularSpeed(0.f)
	, ForwardSpeed(0.f)
	, AccumulatedForce(0.f, 0.f)
	, Mass(10.f)
	, Acceleration(0.f)
	, Velocity(0.f, 0.f)
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

	//if (!Math::NearZero(ForwardSpeed))
	//{
	//	Vector2 NewPosition = Owner->GetPosition();
	//	NewPosition += Owner->GetForward() * ForwardSpeed * DeltaTime;
	//	Owner->SetPosition(NewPosition);
	//}

	Vector2 AccelerationVector = AccumulatedForce * Vector2(1/Mass,1/Mass);
	Velocity += AccelerationVector * DeltaTime;

	Vector2 NextOffsetVector = Velocity * DeltaTime;

	if (Owner)
	{
		Vector2 NewPosition = Owner->GetPosition() + NextOffsetVector;
		Owner->SetPosition(NewPosition);
	}

	ClearForce();
}

void MoveComponent::AddForce(const Vector2& InForce)
{
	AccumulatedForce += InForce;
}