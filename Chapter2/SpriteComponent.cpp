#include "SpriteComponent.h"

#include "Math.h"

SpriteComponent::SpriteComponent(Actor* InOwner, int InDrawOrder)
	: ActorComponent(InOwner)
	, Texture(nullptr)
	, DrawOrder(DrawOrder)
	, Width(0)
	, Height(0)
{
	if (Owner)
	{
		Owner->GetGame()->AddSprite(this);
	}
}

SpriteComponent::~SpriteComponent()
{
	if (Owner)
	{
		Owner->GetGame()->RemoveSprite(this);
	}
}

void SpriteComponent::Draw(SDL_Renderer* Renderer)
{
	if (!Renderer)
		return;

	if (!Texture)
		return;

	if (!Owner)
		return;

	SDL_Rect Rect;
	
	//소유자의 스케일(배율)값으로 너비 / 높이를 스케일링
	Rect.w = static_cast<int>(Width * Owner->GetScale());
	Rect.h = static_cast<int>(Height * Owner->GetScale());

	//소유자의 위치를 중심으로 사각형을 배치
	Rect.x = static_cast<int>(Owner->GetPosition().x - (Rect.w / 2));
	Rect.y = static_cast<int>(Owner->GetPosition().y - (Rect.h / 2));

	//스프라이트를 그린다
	SDL_RenderCopyEx(
		Renderer,
		Texture,
		nullptr,
		&Rect,
		-Math::ToDegrees(Owner->GetRotation()),
		nullptr,
		SDL_FLIP_NONE);
}

void SpriteComponent::SetTexture(SDL_Texture* InTexture)
{
	if (!InTexture)
		return;

	Texture = InTexture;

	//텍스처의 너비와 높이를 얻자
	SDL_QueryTexture(Texture, nullptr, nullptr, &Width, &Height);
}
