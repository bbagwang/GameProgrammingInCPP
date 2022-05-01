// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FPSCamera.h"
#include "Actor.h"

FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner)
	,mPitchSpeed(0.0f)
	,mMaxPitch(Math::Pi / 3.0f)
	,mPitch(0.0f)
{
}

void FPSCamera::Update(float deltaTime)
{
	//�θ� ������Ʈ�� ���� (������ �ƹ��͵� �������� ����.)
	CameraComponent::Update(deltaTime);
	
	//ī�޶� ��ġ�� ������ ��ġ
	Vector3 cameraPos = mOwner->GetPosition();

	//Pitch �ӵ��� Pitch�� ����
	mPitch += mPitchSpeed * deltaTime;
	
	//Pitch ���� [-max, +max] �� ����
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	
	//�������� ���� �� ���Ϳ� ����
	//Pitch ȸ���� ��Ÿ���� ���ʹϾ����� ǥ��
	Quaternion q(mOwner->GetRight(), mPitch);
	
	// Rotate owner forward by pitch quaternion
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	//Ÿ���� �������� 100 ���� ���� �տ� �ִٰ� ����
	Vector3 target = cameraPos + viewForward * 100.0f;
	// Also rotate up by pitch quaternion
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);
	
	//LookAt ����� ������ �� �� ��ķ� ����
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}
