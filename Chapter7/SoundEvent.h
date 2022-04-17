// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <string>
#include "Math.h"

class SoundEvent
{
public:
	SoundEvent();
	//관련 FMOD 이벤트 인스턴스가 존재하면 true를 반환
	bool IsValid();
	//이벤트를 처음부터 재시작
	void Restart();
	//이벤트 정지
	void Stop(bool allowFadeOut = true);
	
	// Setters
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	void SetOcclusion(float directocclusion, float reverbocclusion);

	// Getters
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
	
	// Positional
	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTrans, Vector3 velocity);

protected:
	// Make this constructor protected and AudioSystem a friend
	// so that only AudioSystem can access this constructor.
	//이 생성자는 protected로 선언 그리고 AudioSystem을 friend로 설정
	//이를 통해 오직 AudioSystem만이 이 생성자에 접근할 수 있다.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);

private:
	class AudioSystem* mSystem;
	unsigned int mID;
};
