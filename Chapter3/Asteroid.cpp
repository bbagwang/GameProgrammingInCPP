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
