// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowCamera.h"
#include "Actor.h"

FollowCamera::FollowCamera(Actor* owner)
	:CameraComponent(owner)
	, mHorzDist(350.0f)
	, mVertDist(150.0f)
	, mTargetDist(100.0f)
	, mSpringConstant(64.0f)
	, mOffset(-400.0f, 0.0f, 100.0f)
	, mUp(Vector3::UnitZ)
	, mPitchSpeed(0.0f)
	, mYawSpeed(0.0f)
	, bOrbit(false)
{
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	if (bOrbit)
	{
		//���� ������ ���� ���Ϳ� Yaw�� ����� ���ʹϾ� ����
		Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
		//�����°� ���� ���͸� Yaw ���ʹϾ��� ����ؼ� ��ȯ
		mOffset = Vector3::Transform(mOffset, yaw);
		mUp = Vector3::Transform(mUp, yaw);

		//���� ���ͷκ��� ī�޶��� ����/���� �� ���͸� ���
		//���� ���� => owner.position - (owner.position + offset)
		//= -offset
		Vector3 forward = -1.0f * mOffset;
		forward.Normalize();
		Vector3 right = Vector3::Cross(mUp, forward);
		right.Normalize();

		//ī�޶� ���� �� ���͸� �������� ȸ���ϴ� Pitch ���ʹϾ� ����
		Quaternion pitch(right, mPitchSpeed * deltaTime);
		//ī�޶� �����°� ���� ����(ī�޶��� ���� ���ʹ�)�� Pitch ���ʹϾ����� ȸ����Ŵ
		mOffset = Vector3::Transform(mOffset, pitch);
		mUp = Vector3::Transform(mUp, pitch);

		//��ȯ ����� ���
		Vector3 target = mOwner->GetPosition();
		Vector3 cameraPos = target + mOffset;
		Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
		SetViewMatrix(view);
	}
	else
	{
		//������ ��������κ��� ����(����) ���ڰ� ����Ѵ�.
		float dampening = 2.0f * Math::Sqrt(mSpringConstant);
		//�̻����� ��ġ ����Ѵ�.
		Vector3 idealPos = ComputeCameraPos();
		//�̻����� ��ġ�� ���� ��ġ�� ���� ����Ѵ�.
		Vector3 diff = mActualPos - idealPos;
		//�������� ���ӵ��� ����Ѵ�
		Vector3 acel = (-mSpringConstant * diff) - (dampening * mVelocity);
		//�ӵ��� �����Ѵ�
		mVelocity += acel * deltaTime;
		//���� ī�޶��� ��ġ�� �����Ѵ�.
		mActualPos += mVelocity * deltaTime;
		//Ÿ���� ������ �տ� �ִ� ��� �����̴�.
		Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;

		//�� ����� �����ϱ� ���� �̻����� ��ġ�� �ƴ϶� ���� ��ġ�� ���
		Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

		SetViewMatrix(view);
	}
}

void FollowCamera::SnapToIdeal()
{
	//���� ��ġ�� �̻����� ��ġ�� ����
	mActualPos = ComputeCameraPos();
	//�ӵ��� 0���� �ʱ�ȭ
	mVelocity = Vector3::Zero;
	//Ÿ�� ������ �� ����� ���
	Vector3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,
		Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
	//�������� ���� �� ���ʿ� ī�޶� ��ġ ����
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}
