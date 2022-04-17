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

//FMOD 헤더를 헤더에서 참조하지 않기 위해, 타입들을 전방선언함.
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
	//로드된 뱅크를 관리하는 맵
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	//이벤트 이름과 EventDescription 맵
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	// Map of event id to EventInstance
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	// Map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	//FMOD 스튜디오 시스템
	FMOD::Studio::System* mSystem;
	//FMOD 저수준 시스템 (거의 접근할 일 없음)
	FMOD::System* mLowLevelSystem;
};