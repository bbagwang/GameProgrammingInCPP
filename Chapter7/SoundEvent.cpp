// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SoundEvent.h"
#include "AudioSystem.h"
#include <fmod_studio.hpp>

SoundEvent::SoundEvent(class AudioSystem* system, unsigned int id)
	:mSystem(system)
	,mID(id)
{
}

SoundEvent::SoundEvent()
	:mSystem(nullptr)
	,mID(0)
{
}

bool SoundEvent::IsValid()
{
	return (mSystem && mSystem->GetEventInstance(mID) != nullptr);
}

void SoundEvent::Restart()
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->start();
	}
}

void SoundEvent::Stop(bool allowFadeOut /* true */)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ?
			FMOD_STUDIO_STOP_ALLOWFADEOUT :
			FMOD_STUDIO_STOP_IMMEDIATE;
		event->stop(mode);
	}
}

void SoundEvent::SetPaused(bool pause)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setPaused(pause);
	}
}

void SoundEvent::SetVolume(float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setVolume(value);
	}
}

void SoundEvent::SetPitch(float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setPitch(value);
	}
}

void SoundEvent::SetParameter(const std::string& name, float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setParameterValue(name.c_str(), value);
	}
}

void SoundEvent::SetOcclusion(float directocclusion, float reverbocclusion)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		//채널 그룹을 사용할 수 있도록 커맨드를 비운다
		mSystem->mSystem->flushCommands();
		//이벤트로부터 채널 그룹을 얻는다
		FMOD::ChannelGroup* ChannelGroup = nullptr;
		event->getChannelGroup(&ChannelGroup);
		//차폐 인자를 설정한다
		//occFactor는 0.0 (차폐 없음)에서 1.0 (완전한 차폐) 값을 가진다.
		ChannelGroup->set3DOcclusion(directocclusion, reverbocclusion);
	}
}

bool SoundEvent::GetPaused() const
{
	bool retVal = false;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getPaused(&retVal);
	}
	return retVal;
}

float SoundEvent::GetVolume() const
{
	float retVal = 0.0f;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getVolume(&retVal);
	}
	return retVal;
}

float SoundEvent::GetPitch() const
{
	float retVal = 0.0f;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getPitch(&retVal);
	}
	return retVal;
}

float SoundEvent::GetParameter(const std::string& name)
{
	float retVal = 0.0f;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getParameterValue(name.c_str(), &retVal);
	}
	return retVal;
}

bool SoundEvent::Is3D() const
{
	bool retVal = false;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		//이벤트 디스크립션을 얻는다.
		FMOD::Studio::EventDescription* ed = nullptr;
		event->getDescription(&ed);
		if (ed)
		{
			//이 이벤트는 3D 인가?
			ed->is3D(&retVal);
		}
	}
	return retVal;
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		//게임좌표	(+x 전방, +y 오른쪽, +z 위쪽) 를
		//FMOD		(+z 전방, +x 오른쪽, +y 위쪽) 좌표로 변환
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void SoundEvent::Set3DAttributes(const Matrix4& worldTrans, Vector3 velocity)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD_3D_ATTRIBUTES attr;
		//위치, 전방, 상향 벡터 설정
		attr.position = VecToFMOD(worldTrans.GetTranslation());
		//세계 공간에서 첫 번째 행은 전방 벡터
		attr.forward = VecToFMOD(worldTrans.GetXAxis());
		//세 번째 행은 상향 벡터
		attr.up = VecToFMOD(worldTrans.GetZAxis());
		//TODO : FIX
		//속도를 0으로 설정 (도플러 효과를 사용한다면 수정)
		attr.velocity = VecToFMOD(velocity);

		event->set3DAttributes(&attr);
	}
}
