#pragma once

struct Vector2
{
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float InX, float InY) : x(InX), y(InY) {}
	Vector2(const Vector2& InVec) : x(InVec.x), y(InVec.y) {}

public:
	float x;
	float y;
};

