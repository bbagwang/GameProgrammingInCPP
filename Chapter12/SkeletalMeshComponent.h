// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"
#include <vector>
#include <string>

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// Draw this mesh component
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f, float BlendTime = 0.25f);

	//현재 포즈의 특정 본의 오브젝트 공간 위치를 반환
	Vector3 GetBonePosition(std::string BoneName);

protected:
	void ComputeMatrixPalette();

	MatrixPalette mPalette;
	const class Skeleton* mSkeleton;
	const class Animation* mAnimation;
	const class Animation* mBlendBaseAnimation;

	float mAnimPlayRate;
	float mAnimTime;
	float mBlendTime;
	float mBlendBaseAnimTime;
	float mCurrentBlendTime;
	float mCurrentBlendWeight;

	bool bBlending;

	std::vector<Matrix4> mCurrentPoses;
	std::vector<Matrix4> mBlendBasePoses;
};
