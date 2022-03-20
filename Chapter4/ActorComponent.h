#pragma once

#include "Actor.h"

class ActorComponent
{
public:
	//업데이트 순서값이 작을수록 컴포넌트가 더 빨리 갱신된다.
	ActorComponent(Actor* InOwner, int InUpdateOrder = 100);
	virtual ~ActorComponent();

	//델타 시간으로 이 컴포넌트를 업데이트
	virtual void Update(float DeltaTime);
	
	//컴포넌트 입력 처리
	virtual void ProcessInput(const uint8_t* KeyState) {}

	inline int GetUpdateOrder() const { return UpdateOrder; }

protected:
	//소유자 액터
	Actor* Owner = nullptr;
	//컴포넌트의 업데이트 순서
	int UpdateOrder = 100;
};

