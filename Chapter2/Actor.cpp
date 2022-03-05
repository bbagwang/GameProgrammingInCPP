#include "Actor.h"

#include "ActorComponent.h"

Actor::Actor(Game* InGame)
	: State(ActorState::Active)
	, Position()
	, Scale(1.f)
	, Rotation(0.f)
	, Components()
	, game(InGame)
{
	if (game)
	{
		game->AddActor(this);
	}
}

Actor::~Actor()
{
	if (game)
	{
		game->RemoveActor(this);
	}

	//컴포넌트 삭제 필요
	//컴포넌트 소멸자에서 RemoveComponent 호출되어서, 다른 방식의 루프가 필요함
	while (!Components.empty())
	{
		delete Components.back();
	}
}

void Actor::Update(float DeltaTime)
{
	if (State == ActorState::Active)
	{
		//컴포넌트 먼저 업데이트
		UpdateComponents(DeltaTime);
		//액터 로직 업데이트
		UpdateActor(DeltaTime);
	}
}

void Actor::UpdateComponents(float DeltaTime)
{
	for (auto Component : Components)
	{
		if (Component)
		{
			Component->Update(DeltaTime);
		}
	}
}

void Actor::UpdateActor(float DeltaTime)
{
}

void Actor::AddComponent(ActorComponent* Component)
{
	if (!Component)
		return;
	
	//정렬된 벡터에서 넣을 위치를 찾는다
	//넣으려는 컴포넌트보다 Order가 높은 곳을 찾음
	int MyOrder = Component->GetUpdateOrder();
	
	auto Iter = Components.begin();
	for (; Iter != Components.end(); ++Iter)
	{
		if (MyOrder < (*Iter)->GetUpdateOrder())
		{
			break;
		}
	}

	//찾은 위치 앞에 넣어줌
	Components.insert(Iter, Component);
}

void Actor::RemoveComponent(ActorComponent* Component)
{
	if (!Component)
		return;

	auto Iter = std::find(Components.begin(), Components.end(), Component);
	if (Iter != Components.end())
	{
		Components.erase(Iter);
	}
}
