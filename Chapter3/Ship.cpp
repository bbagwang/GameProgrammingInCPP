#include "Ship.h"

#include "InputComponent.h"
#include "Laser.h"
#include "SpriteComponent.h"

Ship::Ship(Game* InGame)
	: Actor(InGame)
{
	SpriteComponent* SpriteComp = new SpriteComponent(this, 150);
	SpriteComp->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));

	//인풋 컴포넌트 생성 및 초기화
	InputComponent* InputComp = new InputComponent(this);
	InputComp->SetForwardKey(SDL_SCANCODE_W);
	InputComp->SetBackwardKey(SDL_SCANCODE_S);
	InputComp->SetClockwiseKey(SDL_SCANCODE_A);
	InputComp->SetCounterClockwiseKey(SDL_SCANCODE_D);
	InputComp->SetMaxForwardSpeed(300.f);
	InputComp->SetMaxAngularSpeed(Math::TwoPi);
}

void Ship::UpdateActor(float DeltaTime)
{
	LaserCooldown -= DeltaTime;
}

void Ship::ActorInput(const uint8_t* KeyState)
{
	if (KeyState[SDL_SCANCODE_SPACE] && LaserCooldown <= 0.0f)
	{
		//레이저 액터를 만들고 Ship의 액터와 같은 위치, 회전 값 초기화
		Laser* LaserActor = new Laser(GetGame());
		LaserActor->SetPosition(GetPosition());
		LaserActor->SetRotation(GetRotation());

		//레이저 쿨타임 리셋
		LaserCooldown = 0.5f;
	}
}
