// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FPSActor.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"

FPSActor::FPSActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);

	mCameraComp = new FPSCamera(this);

	mFPSModel = new Actor(game);
	mFPSModel->SetScale(0.75f);
	mMeshComp = new MeshComponent(mFPSModel);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Play the footstep if we're moving and haven't recently
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}
	
	//액터 위치에 대한 FPS 모델의 상대적인 위치 갱신
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);
	
	//액터 회전값으로 회전값을 초기화
	Quaternion q = GetRotation();
	//카메라의 Pitch 값으로 회전
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), mCameraComp->GetPitch()));
	mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// Mouse movement
	//SDL로 부터 상대적인 마우스 이동값을 얻는다
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	//마우스 이동은 -500 에서 +500 사이의 값이라고 추정
	const int maxMouseSpeed = 500;
	//초당 회전의 최대 속도
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		//[-1.0, 1.0] 범위로 변환
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		//초당 회전을 곱한다.
		angularSpeed *= maxAngularSpeed;
	}
	mMoveComp->SetAngularSpeed(angularSpeed);

	// Compute pitch
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		// Convert to ~[-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
	// Pause here because the way I setup the parameter in FMOD
	// changing it will play a footstep
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
