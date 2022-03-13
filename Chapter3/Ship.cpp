#include "Ship.h"

#include "InputComponent.h"
#include "Laser.h"
#include "SpriteComponent.h"

Ship::Ship(Game* InGame)
	: Actor(InGame)
{
	SpriteComponent* SpriteComp = new SpriteComponent(this, 150);
	SpriteComp->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));

	//��ǲ ������Ʈ ���� �� �ʱ�ȭ
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
		//������ ���͸� ����� Ship�� ���Ϳ� ���� ��ġ, ȸ�� �� �ʱ�ȭ
		Laser* LaserActor = new Laser(GetGame());
		LaserActor->SetPosition(GetPosition());
		LaserActor->SetRotation(GetRotation());

		//������ ��Ÿ�� ����
		LaserCooldown = 0.5f;
	}
}
