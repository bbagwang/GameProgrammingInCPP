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

	//Game ���� ȣ���ϴ� Update �Լ�
	void Update(float DeltaTime);
	//Actor �� ������ ��� ActorComponent�� ������Ʈ
	void UpdateComponents(float DeltaTime);
	//Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ�
	virtual void UpdateActor(float DeltaTime);

	//Getters / Setters
	inline ActorState GetState() const { return State; }
	inline void SetState(const ActorState NewState) { State = NewState; }

	inline const Vector2& GetPosition() const { return Position; }
	inline void SetPosition(Vector2 InPosition) { Position = InPosition; }
	inline float GetScale() const { return Scale; }
	inline void SetScale(float InScale) { Scale = InScale; }
	inline float GetRotation() const { return Rotation; }
	inline void SetRotation(float InRotation) { Rotation = InRotation; }

	inline const std::vector<ActorComponent*>& GetComponents() const { return Components; }
	inline Game* GetGame() const { return game; }

	//������Ʈ �߰� / ����
	void AddComponent(ActorComponent* Component);
	void RemoveComponent(ActorComponent* Component);

protected:
	//���� ����
	ActorState State;
	
	//Transform ����
	//������ �߽���
	Vector2 Position;
	//������ ���� (100%�� ��� 1.0f)
	float Scale;
	//������ ȸ�� ���� (����)
	float Rotation;

	//�� ���Ͱ� ������ ������Ʈ��
	std::vector<ActorComponent*> Components;
	
	//���� ��ü
	Game* game;
};
