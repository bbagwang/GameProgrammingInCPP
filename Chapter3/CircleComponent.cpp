#include "CircleComponent.h"

CircleComponent::CircleComponent(Actor* InOwner)
	: ActorComponent(InOwner)
	, Radius(0.f)
{
}

float CircleComponent::GetRadius() const
{
	return Owner->GetScale() * Radius;
}

const Vector2& CircleComponent::GetCenter() const
{
	return Owner->GetPosition();
}

bool Intersect(const CircleComponent& A, const CircleComponent& B)
{
	//거리 제곱값을 계산
	
	Vector2 DiffVector = A.GetCenter() - B.GetCenter();
	float DistanceSquared = DiffVector.LengthSq();

	//반지름 합의 제곱값을 계산
	float RadiusSquared = A.GetRadius() + B.GetRadius();
	RadiusSquared *= RadiusSquared;

	return DistanceSquared <= RadiusSquared;
}
