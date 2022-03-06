#include "Character.h"

#include "AnimSpriteComponent.h"

Character::Character(Game* InGame)
	: Actor(InGame)
{
	ASC = new AnimSpriteComponent(this, 1004);
	std::vector<SDL_Texture*> Anims = {
		game->GetTexture("Assets/Character01.png"),//Move Start
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),//Move End
		game->GetTexture("Assets/Character07.png"),//Jump Start
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),//Jump End
		game->GetTexture("Assets/Character16.png"),//Attack Start
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png") //Attack End
	};

	ASC->SetAnimTextures(Anims);

	ASC->AddAnimSet(AnimState::Idle, AnimSet(0,0));
	ASC->AddAnimSet(AnimState::Move, AnimSet(0,5));
	ASC->AddAnimSet(AnimState::Jump, AnimSet(6,14));
	ASC->AddAnimSet(AnimState::Punch, AnimSet(15,17));

	ASC->SetAnimSet(AnimState::Idle);
}

void Character::UpdateActor(float DeltaTime)
{
	static const int SCREEN_RESTRICT = 25;
	static const int SCREEN_WIDTH = 1024;
	static constexpr int SCREEN_WIDTH_RESTRICT = SCREEN_WIDTH - SCREEN_RESTRICT;
	static const int SCREEN_HEIGHT = 768;
	static constexpr int SCREEN_HEIGHT_RESTRICT = (SCREEN_HEIGHT - SCREEN_RESTRICT*2.5);

	if (bJumpingState)
	{
		Velocity.y += GravityScale * DeltaTime;
	}

	//속도와 델타 시간으로 위치를 갱신
	Vector2 NewPosition = GetPosition();
	NewPosition.x += Velocity.x * DeltaTime;
	NewPosition.y += Velocity.y * DeltaTime;
	
	//화면의 왼쪽 반으로 위치를 제한
	if (NewPosition.x < SCREEN_RESTRICT)
	{
		NewPosition.x = SCREEN_RESTRICT;
	}
	else if (NewPosition.x > SCREEN_WIDTH_RESTRICT)
	{
		NewPosition.x = SCREEN_WIDTH_RESTRICT;
	}

	if (NewPosition.y < SCREEN_RESTRICT)
	{
		NewPosition.y = SCREEN_RESTRICT;
	}
	else if (NewPosition.y > SCREEN_HEIGHT_RESTRICT)
	{
		NewPosition.y = SCREEN_HEIGHT_RESTRICT;

		if (bJumpingState)
		{
			bJumpingState = false;
			Velocity.y = 0.f;
			SetAnimState(AnimState::Idle);
		}
	}

	SetPosition(NewPosition);
}

void Character::ProcessKeyboard(const uint8_t* State)
{
	static const float RIGHT_SPEED_VALUE = 250.f;

	Velocity.x = 0;

	//좌/우
	if (State[SDL_SCANCODE_D])
	{
			Velocity.x += RIGHT_SPEED_VALUE;
	}
	if (State[SDL_SCANCODE_A])
	{
			Velocity.x -= RIGHT_SPEED_VALUE;
	}

	if (Velocity.x != 0 && (GetAnimState()!=AnimState::Move))
	{
		SetAnimState(AnimState::Move);
	}
	else if (Velocity.x == 0 && GetAnimState() != AnimState::Idle)
	{
		SetAnimState(AnimState::Idle);
	}

	//상/하
	if (State[SDL_SCANCODE_W])
	{
		if (!bJumpingState)
		{
			Velocity.y -= JumpScale;
			SetAnimState(AnimState::Jump);
			bJumpingState = true;
		}
	}

	if (State[SDL_SCANCODE_SPACE])
	{
		if(GetAnimState() != AnimState::Punch)
			SetAnimState(AnimState::Punch);
	}
}

void Character::SetAnimState(AnimState NewState)
{
	if (!ASC)
		return;

	ASC->SetAnimSet(NewState);
}

AnimState Character::GetAnimState() const
{
	if (!ASC)
		return AnimState::Idle;

	return AnimState(ASC->GetAnimSetIndex());
}

