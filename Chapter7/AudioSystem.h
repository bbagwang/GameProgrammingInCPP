// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

//FMOD ����� ������� �������� �ʱ� ����, Ÿ�Ե��� ���漱����.
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();

	// Load/unload banks
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	// For positional audio
	void SetListener(const Matrix4& viewMatrix, Vector3 velocity);
	// Control buses
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	// Tracks the next ID to use for event instances
	static unsigned int sNextID;

	class Game* mGame;
	//�ε�� ��ũ�� �����ϴ� ��
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	//�̺�Ʈ �̸��� EventDescription ��
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	// Map of event id to EventInstance
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	// Map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	//FMOD ��Ʃ��� �ý���
	FMOD::Studio::System* mSystem;
	//FMOD ������ �ý��� (���� ������ �� ����)
	FMOD::System* mLowLevelSystem;
};