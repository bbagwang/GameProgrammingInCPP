#pragma once

#include "Actor.h"
#include "SoundEvent.h"

class DopplerActor : public Actor
{
public:
	DopplerActor(class Game* game);

	virtual void UpdateActor(float deltaTime);

protected:
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
	class MoveComponent* mMoveComp;

	SoundEvent mFireSoundEvent;

	bool bForward;
	float FlipTime;
	float FlipTimeElapsed;
};

