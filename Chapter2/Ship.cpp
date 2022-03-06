#include "Ship.h"

#include "AnimSpriteComponent.h"

Ship::Ship(Game* InGame)
	: Actor(InGame)
	, RightSpeed(0.f)
	, DownSpeed(0.f)
{
	AnimSpriteComponent* ASC = new AnimSpriteComponent(this, 1004);
	std::vector<SDL_Texture*> Anims = {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
	};

	ASC->SetAnimTextures(Anims);
}

void Ship::UpdateActor(float DeltaTime)
{
	Actor::UpdateActor(DeltaTime);

	//�ӵ��� ��Ÿ �ð����� ��ġ�� ����
	Vector2 NewPosition = GetPosition();
	NewPosition.x += RightSpeed * DeltaTime;
	NewPosition.y += DownSpeed * DeltaTime;
	
	static const int SCREEN_RESTRICT = 25;
	static const int SCREEN_WIDTH = 1024;
	static constexpr int SCREEN_WIDTH_LEFT_HALF = (SCREEN_WIDTH / 2) - SCREEN_RESTRICT;
	static const int SCREEN_HEIGHT = 768;
	static constexpr int SCREEN_HEIGHT_RIGHT_RESTRICT = (SCREEN_HEIGHT - SCREEN_RESTRICT);
	
	//ȭ���� ���� ������ ��ġ�� ����
	if (NewPosition.x < SCREEN_RESTRICT)
	{
		NewPosition.x = SCREEN_RESTRICT;
	}
	else if (NewPosition.x > SCREEN_WIDTH_LEFT_HALF)
	{
		NewPosition.x = SCREEN_WIDTH_LEFT_HALF;
	}
	
	if (NewPosition.y < SCREEN_RESTRICT)
	{
		NewPosition.y = SCREEN_RESTRICT;
	}
	else if (NewPosition.y > SCREEN_HEIGHT_RIGHT_RESTRICT)
	{
		NewPosition.y = SCREEN_HEIGHT_RIGHT_RESTRICT;
	}

	SetPosition(NewPosition);
}

void Ship::ProcessKeyboard(const uint8_t* State)
{
	//�Է� �ʱ�ȭ
	RightSpeed = 0.f;
	DownSpeed = 0.f;

	static const float RIGHT_SPEED_VALUE = 250.f;
	static const float DOWN_SPEED_VALUE = 300.f;

	//��/��
	if (State[SDL_SCANCODE_D])
	{
		RightSpeed += RIGHT_SPEED_VALUE;
	}
	if (State[SDL_SCANCODE_A])
	{
		RightSpeed -= RIGHT_SPEED_VALUE;
	}

	//��/��
	if (State[SDL_SCANCODE_S])
	{
		DownSpeed += DOWN_SPEED_VALUE;
	}
	if (State[SDL_SCANCODE_W])
	{
		DownSpeed -= DOWN_SPEED_VALUE;
	}
}
