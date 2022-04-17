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
	//������ ���� �α��� ����
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, //������ ��츸 �α׸� ����
		FMOD_DEBUG_MODE_TTY //stdout���� �α� ���
	);

	//FMOD STUDIO �ý��� ��ü �ν��Ͻ� ����
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//FMOD �ʱ�ȭ
	result = mSystem->initialize(
		512, //���ÿ� ����� �� �ִ� ������ �ִ� ����
		FMOD_STUDIO_INIT_NORMAL, //�⺻ ���� ���
		FMOD_INIT_NORMAL, //�⺻ ����
		nullptr //��κ� nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//�ʱ�ȭ �Ϸ��� ������ �ý��� �����͸� �� ����
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	mLowLevelSystem->set3DSettings(
		1.f,	//���÷� ������, 1 = ����, 1���� �� ũ�� ����� �Ҹ��� ����
		50.f,	//���� ������ ũ�� = 1���� (7�常 50 ���� ����)
		1.f		//(���÷��� �������, 1�� ���ܵд�.)
	);

	//������ ��ũ �ε� (Master Bank.string.bank �� ���� �ε�)
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
	//�� �� �ε����� �ʰ� �Ѵ�
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	//��ũ �ε� �õ�
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(), //��ũ�� ���� �̸�
		FMOD_STUDIO_LOAD_BANK_NORMAL, //�Ϲ����� ������� �ε�
		&bank //��ũ ������ ����
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		//��ũ�� �ʿ� �߰�
		mBanks.emplace(name, bank);
		//��Ʈ���� ������ �ƴ� ��� ���� �����͸� �ε�
		bank->loadSampleData();
		//��ũ�� �̺�Ʈ ���� ��´�.
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			//��ũ���� EventDescription ����Ʈ�� ��´�.
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				//event:/Explosion2D ���� �̺�Ʈ�� ��θ� ��´�.
				e->getPath(eventName, maxPathLength, nullptr);
				//�̺�Ʈ�� �ʿ� �߰��Ѵ�.
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
	//�̹� �ε�Ǿ����� ������ ����
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	//��ũ�� �ִ� ��� �̺�Ʈ ����
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		//��ũ�� ���� EventDescription ���� ������
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//�̺�Ʈ ����Ʈ�� ������
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			//�̺�Ʈ ��θ� ������
			e->getPath(eventName, 512, nullptr);
			//�̺�Ʈ�� ������.
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
	//�̺�Ʈ�� �����ϴ��� Ȯ��
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		//�̺�Ʈ �ν��Ͻ� ����
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			//�̺�Ʈ �ν��Ͻ� ����
			event->start();
			//�� ���̵� ��� �ʿ� �߰��Ѵ�.
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
	//������ �̺�Ʈ�� ã�´�.
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		//�� �̺�Ʈ�� ���°��� ��´�.
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			//�̺�Ʈ�� �����ϰ� id�� done �ʿ� �߰��Ѵ�.
			e->release();
			done.emplace_back(iter.first);
		}
	}
	
	//�Ϸ�� �̺�Ʈ �ν��Ͻ��� �ʿ��� ����
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	//FMOD ������Ʈ
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
	//�� ����� ������� ���ؼ� �ʿ�� �ϴ� ���� ������ ���͸� ��´�.
	Matrix4 invView = viewMatrix;
	invView.Invert();

	FMOD_3D_ATTRIBUTES listener;
	
	//��ġ�� ���� ����, ���� ���͸� ����
	
	//�� ����� ����Ŀ��� �� ��° ��(GetTranslation ���� ���� �� ����)��
	//ó�� �� ��Ҵ� ī�޶��� ���� ���� ��ġ�� �ش��Ѵ�.
	listener.position = VecToFMOD(invView.GetTranslation());
	
	//����° ��(GetZAxis �� ���� �� ����) ��
	//ó�� �� ��Ҵ� ���� ����
	listener.forward = VecToFMOD(invView.GetZAxis());
	
	//�ι�° ��(GetYAxis �� ���� �� ����) ��
	//ó�� �� ��Ҵ� ���� ����
	listener.up = VecToFMOD(invView.GetYAxis());
	
	//TODO : FIX
	//�ӵ��� 0���� ���� (���÷� ȿ���� ����� �� ����)
	listener.velocity = VecToFMOD(velocity);
	
	//FMOD�� ������ (0 = ������ �ε��� ��ȣ. ������ �ϳ��ϱ� 0���� ����)
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
