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
	//���� FMOD �̺�Ʈ �ν��Ͻ��� �����ϸ� true�� ��ȯ
	bool IsValid();
	//�̺�Ʈ�� ó������ �����
	void Restart();
	//�̺�Ʈ ����
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
	//�� �����ڴ� protected�� ���� �׸��� AudioSystem�� friend�� ����
	//�̸� ���� ���� AudioSystem���� �� �����ڿ� ������ �� �ִ�.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);

private:
	class AudioSystem* mSystem;
	unsigned int mID;
};
