#include "Asteroid.h"

#include "CircleComponent.h"
#include "Math.h"
#include "MoveComponent.h"
#include "Random.h"
#include "SpriteComponent.h"

Asteroid::Asteroid(Game* InGame)
	: Actor(InGame)
{
	//랜덤하게 위치와 방향을 초기화한다.
	Vector2 RandomPosition = Random::GetVector(Vector2::Zero, Vector2(1024.f, 768.f));
	SetPosition(RandomPosition);
	SetRotation(Random::GetFloatRange(0.f, Math::TwoPi));

	//스프라이트 컴포넌트를 생성하고 텍스처를 설정한다.
	SpriteComponent* SpriteComp = new SpriteComponent(this);
	SpriteComp->SetTexture(GetGame()->GetTexture("Assets/Asteroid.png"));

	//이동 컴포넌트를 생성하고 전방 속도를 설정한다.
	MoveComponent* MoveComp = new MoveComponent(this);
	MoveComp->SetForwardSpeed(150.f);

	//원 컴포넌트를 생성하고 충돌 체크를 위해 반지름을 설정한다.
	CircleComp = new CircleComponent(this);
	CircleComp->SetRadius(40.f);

	GetGame()->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
	Actor::~Actor();
}
