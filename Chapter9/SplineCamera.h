// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline
{
	//스플라인에 대한 제어점(Control Point)
	//(세그먼트상에 n개의 점이 있다면 총 n+2개의 점이 필요하다.)
	std::vector<Vector3> mControlPoints;
	
	//startIdx = P1 인 스플라인 세그먼트가 주어졌을 때 t 값을 토대로 위치를 계산한다.
	Vector3 Compute(size_t startIdx, float t) const;
	
	size_t GetNumPoints() const { return mControlPoints.size(); }
};

class SplineCamera : public CameraComponent
{
public:
	SplineCamera(class Actor* owner);

	void Update(float deltaTime) override;
	// Restart the spline
	void Restart();

	void SetSpeed(float speed) { mSpeed = speed; }
	void SetSpline(const Spline& spline) { mPath = spline; }
	void SetPaused(bool pause) { mPaused = pause; }

private:
	//카메라가 따라가는 스플라인 경로
	Spline mPath;
	//현재 제어점 인덱스 및 t 값
	size_t mIndex;
	float mT;
	//초당 t의 변화율
	float mSpeed;
	//경로를 따라 카메라가 이동해야 하는지의 여부
	bool mPaused;
};
