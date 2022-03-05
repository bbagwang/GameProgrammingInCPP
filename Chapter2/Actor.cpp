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

	//������Ʈ ���� �ʿ�
	//������Ʈ �Ҹ��ڿ��� RemoveComponent ȣ��Ǿ, �ٸ� ����� ������ �ʿ���
	while (!Components.empty())
	{
		delete Components.back();
	}
}

void Actor::Update(float DeltaTime)
{
	if (State == ActorState::Active)
	{
		//������Ʈ ���� ������Ʈ
		UpdateComponents(DeltaTime);
		//���� ���� ������Ʈ
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
	
	//���ĵ� ���Ϳ��� ���� ��ġ�� ã�´�
	//�������� ������Ʈ���� Order�� ���� ���� ã��
	int MyOrder = Component->GetUpdateOrder();
	
	auto Iter = Components.begin();
	for (; Iter != Components.end(); ++Iter)
	{
		if (MyOrder < (*Iter)->GetUpdateOrder())
		{
			break;
		}
	}

	//ã�� ��ġ �տ� �־���
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
