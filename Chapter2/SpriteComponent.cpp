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
	
	//�������� ������(����)������ �ʺ� / ���̸� �����ϸ�
	Rect.w = static_cast<int>(Width * Owner->GetScale());
	Rect.h = static_cast<int>(Height * Owner->GetScale());

	//�������� ��ġ�� �߽����� �簢���� ��ġ
	Rect.x = static_cast<int>(Owner->GetPosition().x - (Rect.w / 2));
	Rect.y = static_cast<int>(Owner->GetPosition().y - (Rect.h / 2));

	//��������Ʈ�� �׸���
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

	//�ؽ�ó�� �ʺ�� ���̸� ����
	SDL_QueryTexture(Texture, nullptr, nullptr, &Width, &Height);
}
