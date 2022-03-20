#pragma once

#include <vector>

#include "ActorState.h"
#include "Game.h"
#include "Math.h"

class ActorComponent;

class Actor
{
public:
	Actor() = default;
	Actor(Game* InGame);
	virtual ~Actor();

	//Game 에서 호출하는 Update 함수
	void Update(float DeltaTime);
	//Actor 에 부착된 모든 ActorComponent를 업데이트
	void UpdateComponents(float DeltaTime);
	//특정 액터에 특화된 업데이트 코드
	virtual void UpdateActor(float DeltaTime);

	//Game 에서 인풋 처리를 위해 호출하는 비가상함수
	void ProcessInput(const uint8_t* KeyState);
	//Actor 에서 인풋 처리를 위해 호출하는 가상함수
	virtual void ActorInput(const uint8_t* KeyState);

	//Getters / Setters
	inline ActorState GetState() const { return State; }
	inline void SetState(const ActorState NewState) { State = NewState; }

	inline const Vector2& GetPosition() const { return Position; }
	inline void SetPosition(Vector2 InPosition) { Position = InPosition; }
	inline float GetScale() const { return Scale; }
	inline void SetScale(float InScale) { Scale = InScale; }
	inline float GetRotation() const { return Rotation; }
	inline void SetRotation(float InRotation) { Rotation = InRotation; }

	inline Vector2 GetForward() const { return Vector2(Math::Cos(Rotation), -Math::Sin(Rotation)); }

	inline const std::vector<ActorComponent*>& GetComponents() const { return Components; }
	inline Game* GetGame() const { return game; }

	//컴포넌트 추가 / 제거
	void AddComponent(ActorComponent* Component);
	void RemoveComponent(ActorComponent* Component);

protected:
	//액터 상태
	ActorState State;
	
	//Transform 선언
	//액터의 중심점
	Vector2 Position;
	//액터의 배율 (100%인 경우 1.0f)
	float Scale;
	//액터의 회전 각도 (라디안)
	float Rotation;

	//이 액터가 보유한 컴포넌트들
	std::vector<ActorComponent*> Components;
	
	//게임 객체
	Game* game;
};
