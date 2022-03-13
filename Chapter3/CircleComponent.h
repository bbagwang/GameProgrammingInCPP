#pragma once

#include "ActorComponent.h"

#include "Math.h"

class CircleComponent : public ActorComponent
{
public:
	CircleComponent(Actor* InOwner);
	virtual ~CircleComponent() = default;

	inline void SetRadius(float NewRadius) { Radius = NewRadius; }
	float GetRadius() const;

	const Vector2& GetCenter() const;

private:
	float Radius;
};

bool Intersect(const CircleComponent& A, const CircleComponent& B);