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

	//시간이 종료되면 레이저를 삭제
	DeathTimer -= DeltaTime;
	if (DeathTimer <= 0.f)
	{
		SetState(ActorState::Dead);
	}
	else
	{
		//운석과 충돌했는지 체크
		for (auto& AsteroidIter : GetGame()->GetAsteroids())
		{
			if (Intersect(*CircleComp, *AsteroidIter->GetCircle()))
			{
				//하나라도 충돌했다면, 운석과 레이저를 삭제 처리
				SetState(ActorState::Dead);
				AsteroidIter->SetState(ActorState::Dead);
				break;
			}
		}
	}
}
