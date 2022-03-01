#pragma once
#include "PCH.h"

struct Ball
{
public:
	Ball() {}
	Ball(Vector2 InPosition, Vector2 InVelocity)
		: Position(InPosition), Velocity(InVelocity) {}

public:
	Vector2 Position;
	Vector2 Velocity;
};

