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
	,mHorzDist(350.0f)
	,mVertDist(150.0f)
	,mTargetDist(100.0f)
	,mSpringConstant(64.0f)
{
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	//스프링 상수값으로부터 댐핑(감쇄) 인자값 계산한다.
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);
	//이상적인 위치 계산한다.
	Vector3 idealPos = ComputeCameraPos();
	//이상적인 위치와 실제 위치의 차를 계산한다.
	Vector3 diff = mActualPos - idealPos;
	//스프링의 가속도를 계산한다
	Vector3 acel = (-mSpringConstant * diff) - (dampening * mVelocity);
	//속도를 갱신한다
	mVelocity += acel * deltaTime;
	//실제 카메라의 위치를 갱신한다.
	mActualPos += mVelocity * deltaTime;
	//타깃은 소유자 앞에 있는 대상 지점이다.
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;

	//뷰 행렬을 생성하기 위해 이상적인 위치가 아니라 실제 위치를 사용
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	
	SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal()
{
	//실제 위치를 이상적인 위치로 설정
	mActualPos = ComputeCameraPos();
	//속도를 0으로 초기화
	mVelocity = Vector3::Zero;
	//타깃 지점과 뷰 행렬을 계산
	Vector3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,
		Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
	//소유자의 뒤쪽 및 위쪽에 카메라 위치 설정
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}
