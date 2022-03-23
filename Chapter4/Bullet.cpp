#include "Bullet.h"

#include "CircleComponent.h"
#include "Enemy.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Bullet::Bullet(class Game* game)
	:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.0f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(5.0f);

	mLiveTime = 1.0f;
}

void Bullet::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Check for collision vs enemies
	for (Enemy* e : GetGame()->GetEnemies())
	{
		if (Intersect(*mCircle, *(e->GetCircle())))
		{
			// We both die on collision
			e->SetState(ActorState::Dead);
			SetState(ActorState::Dead);
			break;
		}
	}

	mLiveTime -= deltaTime;
	if (mLiveTime <= 0.0f)
	{
		// Time limit hit, die
		SetState(ActorState::Dead);
	}
}