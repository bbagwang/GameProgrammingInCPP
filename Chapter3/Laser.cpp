#include "Laser.h"

#include "Asteroid.h"
#include "CircleComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Laser::Laser(Game* InGame)
	: Actor(InGame)
	, DeathTimer(1.f)
{
	SpriteComponent* SpriteComp = new SpriteComponent(this);
	SpriteComp->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));

	MoveComponent* MoveComp = new MoveComponent(this);
	MoveComp->SetForwardSpeed(800.f);

	CircleComp = new CircleComponent(this);
	CircleComp->SetRadius(11.f);
}

void Laser::UpdateActor(float DeltaTime)
{
	if (GetState() == ActorState::Dead)
		return;

	//�ð��� ����Ǹ� �������� ����
	DeathTimer -= DeltaTime;
	if (DeathTimer <= 0.f)
	{
		SetState(ActorState::Dead);
	}
	else
	{
		//��� �浹�ߴ��� üũ
		for (auto& AsteroidIter : GetGame()->GetAsteroids())
		{
			if (Intersect(*CircleComp, *AsteroidIter->GetCircle()))
			{
				//�ϳ��� �浹�ߴٸ�, ��� �������� ���� ó��
				SetState(ActorState::Dead);
				AsteroidIter->SetState(ActorState::Dead);
				break;
			}
		}
	}
}
