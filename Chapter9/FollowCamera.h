// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;
	
	void SnapToIdeal();

	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }
	bool GetOrbit() const { return bOrbit; }

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }

	void SetOrbit(bool bUse) { bOrbit = bUse; }
	
private:
	Vector3 ComputeCameraPos() const;

	// Actual position of camera
	Vector3 mActualPos;
	// Velocity of actual camera
	Vector3 mVelocity;
	// Horizontal follow distance
	float mHorzDist;
	// Vertical follow distance
	float mVertDist;
	// Target distance
	float mTargetDist;
	// Spring constant (higher is more stiff)
	float mSpringConstant;

	//������ ������
	Vector3 mOffset;
	//ī�޶� ���� ����
	Vector3 mUp;
	//��ġ�� �ʴ� ȸ�� �ӵ�
	float mPitchSpeed;
	//���� �ִ� ȸ�� �ӵ�
	float mYawSpeed;	

	bool bOrbit;
};
