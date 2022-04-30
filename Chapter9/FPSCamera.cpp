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
	//부모 컴포넌트를 갱신 (지금은 아무것도 갱신하지 않음.)
	CameraComponent::Update(deltaTime);
	
	//카메라 위치는 소유자 위치
	Vector3 cameraPos = mOwner->GetPosition();

	//Pitch 속도로 Pitch를 갱신
	mPitch += mPitchSpeed * deltaTime;
	
	//Pitch 값을 [-max, +max] 로 제한
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	
	//소유자의 오른 축 벡터에 대한
	//Pitch 회전을 나타내는 쿼터니언으로 표현
	Quaternion q(mOwner->GetRight(), mPitch);
	
	// Rotate owner forward by pitch quaternion
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	//타겟은 소유자의 100 유닛 단위 앞에 있다고 가정
	Vector3 target = cameraPos + viewForward * 100.0f;
	// Also rotate up by pitch quaternion
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);
	
	//LookAt 행렬을 생성한 뒤 뷰 행렬로 설정
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}
