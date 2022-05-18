// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	float GetStrafeSpeed() const { return mStrafeSpeed; }
	float GetVerticalSpeed() const { return mVerticalSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetVerticalSpeed(float speed) { mVerticalSpeed = speed; }

	void SetGravityEnable(bool bEnable) { bUseGravity = bEnable; }
	bool IsGravityEnable() const { return bUseGravity; }
	
protected:
	float mAngularSpeed;
	float mForwardSpeed;
	float mStrafeSpeed;
	float mVerticalSpeed;
	
	float mGravityAcceleration = 980.f;
	
	bool bUseGravity;
};
