// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	, mPalette()
	, mSkeleton(nullptr)
	, mAnimation(nullptr)
	, mBlendBaseAnimation(nullptr)
	, mAnimPlayRate(1.f)
	, mAnimTime(0.f)
	, mBlendTime(0.f)
	, mBlendBaseAnimTime(0.f)
	, mCurrentBlendTime(0.f)
	, mCurrentBlendWeight(0.f)
	, bBlending(0.f)
	, mCurrentPoses()
	, mBlendBasePoses()
{
}

void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set the matrix palette
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], 
			MAX_SKELETON_BONES);
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		// Wrap around anim time if past duration
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		if (bBlending)
		{
			if (mCurrentBlendWeight > 1.f)
			{
				bBlending = false;
				mBlendBaseAnimation = nullptr;
				mBlendTime = 0.f;
				mBlendBaseAnimTime = 0.f;
				mCurrentBlendTime = 0.f;
				mCurrentBlendWeight = 0.f;
				mBlendBasePoses.empty();
				SDL_Log("StopBlending");
			}
			else
			{
				mCurrentBlendTime += deltaTime;
				mCurrentBlendWeight = mCurrentBlendTime / mBlendTime;
				SDL_Log("%f", mCurrentBlendWeight);
			}
		}

		// Recompute matrix palette
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate, float BlendTime)
{
	if (BlendTime > 0.f && mAnimation)
	{
		SDL_Log("StartBlending");
		bBlending = true;
		mBlendBaseAnimation = mAnimation;
		mBlendTime = BlendTime;
		mBlendBaseAnimTime = mAnimTime;
		mCurrentBlendTime = 0.f;
		mCurrentBlendWeight = 0.f;
		mBlendBasePoses = mCurrentPoses;
	}
	
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;
	
	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}


Vector3 SkeletalMeshComponent::GetBonePosition(std::string BoneName)
{
	const int BoneIndex = mSkeleton->GetBoneIndex(BoneName);
	Vector3 ObjectSpaceLocation = Vector3::Transform(Vector3::Zero, mCurrentPoses[BoneIndex]);
	return Vector3::Transform(ObjectSpaceLocation, GetOwner()->GetWorldTransform());
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();
	
	if (bBlending)
	{
		Animation::GetGlobalBlendPoseAtTime(
			mCurrentPoses,
			mSkeleton,
			mBlendBaseAnimation,
			mBlendBaseAnimTime,
			mAnimation,
			mAnimTime,
			mCurrentBlendWeight);	
	}
	else
	{
	mAnimation->GetGlobalPoseAtTime(mCurrentPoses, mSkeleton, mAnimTime);
	}
	
	

	// Setup the palette for each bone
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
			// Global inverse bind pose matrix times current pose matrix
			mPalette.mEntry[i] = globalInvBindPoses[i] * mCurrentPoses[i];
	}
}
