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
	//�Ÿ� �������� ���
	
	Vector2 DiffVector = A.GetCenter() - B.GetCenter();
	float DistanceSquared = DiffVector.LengthSq();

	//������ ���� �������� ���
	float RadiusSquared = A.GetRadius() + B.GetRadius();
	RadiusSquared *= RadiusSquared;

	return DistanceSquared <= RadiusSquared;
}
