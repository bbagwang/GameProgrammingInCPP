#include "Ship.h"

#include "Asteroid.h"
#include "CircleComponent.h"
#include "InputComponent.h"
#include "Laser.h"
#include "SpriteComponent.h"

Ship::Ship(Game* InGame)
	: Actor(InGame)
	, LaserCooldown(0.5f)
	, TotalInvincibleTime(2.f)
	, CurrentInvincibleTime(0.f)
	, ShipStateValue(ShipState::Normal)
	, SpriteComp(nullptr)
	, CircleComp(nullptr)
	, InputComp(nullptr)
{
	SpriteComp = new SpriteComponent(this, 150);
	SpriteComp->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));

	//��ǲ ������Ʈ ���� �� �ʱ�ȭ
	InputComp = new InputComponent(this);
	InputComp->SetForwardKey(SDL_SCANCODE_W);
	InputComp->SetBackwardKey(SDL_SCANCODE_S);
	InputComp->SetClockwiseKey(SDL_SCANCODE_A);
	InputComp->SetCounterClockwiseKey(SDL_SCANCODE_D);
	InputComp->SetMaxForwardSpeed(300.f);
	InputComp->SetMaxAngularSpeed(Math::TwoPi);

	CircleComp = new CircleComponent(this);
	CircleComp->SetRadius(40.f);
}

void Ship::UpdateActor(float DeltaTime)
{
	LaserCooldown -= DeltaTime;

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

	if (ShipStateValue == ShipState::Crashed)
	{

		CurrentInvincibleTime += DeltaTime;
		if (CurrentInvincibleTime >= TotalInvincibleTime)
		{
			SetCrashedState(false);
		}
		return;
	}

	auto AstroidGame = GetGame();
	if (AstroidGame)
	{
		for (Asteroid* AsteroidActor : AstroidGame->GetAsteroids())
		{
			if (!AsteroidActor || AsteroidActor->GetState() == ActorState::Dead)
				continue;

			if (Intersect(*GetCircle(), *AsteroidActor->GetCircle()))
			{
				OnCollide(AsteroidActor);
				break;
			}
		}
	}
}

void Ship::ActorInput(const uint8_t* KeyState)
{
	if (KeyState[SDL_SCANCODE_SPACE] && LaserCooldown <= 0.0f)
	{
		//������ ���͸� ����� Ship�� ���Ϳ� ���� ��ġ, ȸ�� �� �ʱ�ȭ
		Laser* LaserActor = new Laser(GetGame());
		LaserActor->SetPosition(GetPosition());
		LaserActor->SetRotation(GetRotation());

		//������ ��Ÿ�� ����
		LaserCooldown = 0.5f;
	}
}

void Ship::OnCollide(Actor* CollideActor)
{
	CollideActor->SetState(ActorState::Dead);
	SetCrashedState(true);
}

void Ship::SetCrashedState(bool bNewState)
{
	ShipStateValue = bNewState ? ShipState::Crashed : ShipState::Normal;

	CurrentInvincibleTime = 0.f;

	if (SpriteComp)
	{
		SpriteComp->SetDrawingState(!bNewState);
	}
}
