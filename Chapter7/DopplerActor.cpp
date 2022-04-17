#include "DopplerActor.h"

#include "AudioComponent.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "MoveComponent.h"

DopplerActor::DopplerActor(Game* game)
	: Actor(game)
{
	bForward = true;
	FlipTime = 2.f;
	FlipTimeElapsed = 0.f;

	SetScale(3.0f);
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
	
	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(1000.f);
	Vector3 velocity = mMoveComp->GetForwardSpeed() * GetForward();

	mAudioComp = new AudioComponent(this);
	mAudioComp->SetVelocity(velocity);
	mFireSoundEvent = mAudioComp->PlayEvent("event:/FireLoop");
}

void DopplerActor::UpdateActor(float deltaTime)
{
	FlipTimeElapsed += deltaTime;
	if (FlipTimeElapsed >= FlipTime)
	{
		FlipTimeElapsed = 0.f;

		mMoveComp->SetForwardSpeed(mMoveComp->GetForwardSpeed() * -1.f);
		Vector3 velocity = mMoveComp->GetForwardSpeed() * GetForward();
		mAudioComp->SetVelocity(velocity);
	}
}
