// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "SoundEvent.h"
#include <vector>
#include <string>

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	//액터가 세계 변환 행렬을 계산할 때마다 호출됨
	//변경될 때 마다 mEvents3D에 있는 모든 3D 이벤트의 3D 속성을 갱신
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

	void SetVelocity(Vector3 invelocity);

private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;

	Vector3 velocity;
};