#include "Asteroid.h"

#include "CircleComponent.h"
#include "Math.h"
#include "MoveComponent.h"
#include "Random.h"
#include "SpriteComponent.h"

Asteroid::Asteroid(Game* InGame)
	: Actor(InGame)
{
	//�����ϰ� ��ġ�� ������ �ʱ�ȭ�Ѵ�.
	Vector2 RandomPosition = Random::GetVector(Vector2::Zero, Vector2(1024.f, 768.f));
	SetPosition(RandomPosition);
	SetRotation(Random::GetFloatRange(0.f, Math::TwoPi));

	//��������Ʈ ������Ʈ�� �����ϰ� �ؽ�ó�� �����Ѵ�.
	SpriteComponent* SpriteComp = new SpriteComponent(this);
	SpriteComp->SetTexture(GetGame()->GetTexture("Assets/Asteroid.png"));

	//�̵� ������Ʈ�� �����ϰ� ���� �ӵ��� �����Ѵ�.
	MoveComponent* MoveComp = new MoveComponent(this);
	MoveComp->SetForwardSpeed(150.f);
	MoveComp->AddForce(GetForward() * Random::GetFloatRange(10000.f, 100000.f));
	
	//�� ������Ʈ�� �����ϰ� �浹 üũ�� ���� �������� �����Ѵ�.
	CircleComp = new CircleComponent(this);
	CircleComp->SetRadius(40.f);

	GetGame()->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
	Actor::~Actor();
}

void Asteroid::UpdateActor(float DeltaTime)
{
	Vector2 CurrentPosition = GetPosition();

	if (CurrentPosition.x > 1024.f)
	{
		SetPosition(Vector2(0.f, CurrentPosition.y));
	}
	else if (CurrentPosition.x < 0.f)
	{
		SetPosition(Vector2(1024.f, CurrentPosition.y));
	}

	if (CurrentPosition.y > 768.f)
	{
		SetPosition(Vector2(CurrentPosition.x, 0.f));
	}
	else if (CurrentPosition.y < 0.f)
	{
		SetPosition(Vector2(CurrentPosition.x, 768.f));
	}
}
