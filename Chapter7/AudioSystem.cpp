// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
	,mSystem(nullptr)
	,mLowLevelSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
	//에러에 대한 로깅을 설정
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, //에러일 경우만 로그를 남김
		FMOD_DEBUG_MODE_TTY //stdout으로 로그 출력
	);

	//FMOD STUDIO 시스템 객체 인스턴스 생성
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//FMOD 초기화
	result = mSystem->initialize(
		512, //동시에 출력할 수 있는 사운드의 최대 갯수
		FMOD_STUDIO_INIT_NORMAL, //기본 설정 사용
		FMOD_INIT_NORMAL, //기본 설정
		nullptr //대부분 nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//초기화 완료후 저수준 시스템 포인터를 얻어서 저장
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	mLowLevelSystem->set3DSettings(
		1.f,	//도플러 스케일, 1 = 정상, 1보다 더 크면 과장된 소리를 낸다
		50.f,	//게임 단위의 크기 = 1미터 (7장만 50 으로 설정)
		1.f		//(도플러와 관계없음, 1로 남겨둔다.)
	);

	//마스터 뱅크 로드 (Master Bank.string.bank 를 먼저 로드)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	// Unload all banks
	UnloadAllBanks();
	// Shutdown FMOD system
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name)
{
	//두 번 로딩되지 않게 한다
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	//뱅크 로드 시도
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(), //뱅크의 파일 이름
		FMOD_STUDIO_LOAD_BANK_NORMAL, //일반적인 방식으로 로딩
		&bank //뱅크 포인터 저장
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		//뱅크를 맵에 추가
		mBanks.emplace(name, bank);
		//스트리밍 형식이 아닌 모든 샘플 데이터를 로드
		bank->loadSampleData();
		//뱅크의 이벤트 수를 얻는다.
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			//뱅크에서 EventDescription 리스트를 얻는다.
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				//event:/Explosion2D 같은 이벤트의 경로를 얻는다.
				e->getPath(eventName, maxPathLength, nullptr);
				//이벤트를 맵에 추가한다.
				mEvents.emplace(eventName, e);
			}
		}

		// Get the number of buses in this bank
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// Get list of buses in this bank
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// Get the path of this bus (like bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// Add to buses map
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	//이미 로드되어있지 않으면 리턴
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	//뱅크에 있는 모든 이벤트 해제
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		//뱅크에 대한 EventDescription 들을 가져옴
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//이벤트 리스트를 가져옴
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			//이벤트 경로를 가져옴
			e->getPath(eventName, 512, nullptr);
			//이벤트를 제거함.
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}
	// Get the number of buses in this bank
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		// Get list of buses in this bank
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			// Get the path of this bus (like bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			// Remove this bus
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	// Unload sample data and bank
	bank->unloadSampleData();
	bank->unload();
	// Remove from banks map
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		// Unload the sample data, then the bank itself
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	// No banks means no events
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	//이벤트가 존재하는지 확인
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		//이벤트 인스턴스 생성
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			//이벤트 인스턴스 시작
			event->start();
			//새 아이디를 얻어 맵에 추가한다.
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
	//정지된 이벤트를 찾는다.
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		//이 이벤트의 상태값을 얻는다.
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			//이벤트를 해제하고 id를 done 맵에 추가한다.
			e->release();
			done.emplace_back(iter.first);
		}
	}
	
	//완료된 이벤트 인스턴스를 맵에서 제거
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	//FMOD 업데이트
	mSystem->update();
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// Convert from our coordinates (+x forward, +y right, +z up)
		// to FMOD (+z forward, +x right, +y up)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix, Vector3 velocity)
{
	//뷰 행렬의 역행렬을 구해서 필요로 하는 세계 공간의 벡터를 얻는다.
	Matrix4 invView = viewMatrix;
	invView.Invert();

	FMOD_3D_ATTRIBUTES listener;
	
	//위치와 전방 벡터, 상향 벡터를 설정
	
	//뷰 행렬의 역행렬에서 네 번째 행(GetTranslation 으로 얻을 수 있음)의
	//처음 세 요소는 카메라의 세계 공간 위치에 해당한다.
	listener.position = VecToFMOD(invView.GetTranslation());
	
	//세번째 행(GetZAxis 로 얻을 수 있음) 의
	//처음 세 요소는 전방 벡터
	listener.forward = VecToFMOD(invView.GetZAxis());
	
	//두번째 행(GetYAxis 로 얻을 수 있음) 의
	//처음 세 요소는 상향 벡터
	listener.up = VecToFMOD(invView.GetYAxis());
	
	//TODO : FIX
	//속도를 0으로 설정 (도플러 효과를 사용할 시 수정)
	listener.velocity = VecToFMOD(velocity);
	
	//FMOD로 보낸다 (0 = 리스너 인덱스 번호. 지금은 하나니까 0으로 설정)
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string & name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string & name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}
