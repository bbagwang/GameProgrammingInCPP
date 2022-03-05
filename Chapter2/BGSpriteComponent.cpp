#include "BGSpriteComponent.h"

BGSpriteComponent::BGSpriteComponent(Actor* InOwner, int InDrawOrder)
	: SpriteComponent(InOwner, InDrawOrder)
	, BGTextures()
	, ScreenSize(0.f, 0.f)
	, ScrollSpeed(0.f)
{
}

void BGSpriteComponent::Update(float DeltaTime)
{
	SpriteComponent::Update(DeltaTime);

	for (auto& BGTexture : BGTextures)
	{
		//X ������ ���� ����
		BGTexture.Offset.x += ScrollSpeed * DeltaTime;
		//�� �ؽ�ó�� ȭ���� ������ ����� ������ ����
		//������ ��� �ؽ�ó�� ������ ��ġ�� �ʱ�ȭ�Ѵ�.
		if (BGTexture.Offset.x < -ScreenSize.x)
		{
			BGTexture.Offset.x = ((BGTextures.size() - 1) * ScreenSize.x) - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* Renderer)
{
	if (!Renderer)
		return;

	//������ ��� �ؽ��ĸ� �׸���.
	for (auto& BGTexture : BGTextures)
	{
		SDL_Rect Rect;
		//ȭ�� ũ�� ����
		Rect.w = static_cast<int>(ScreenSize.x);
		Rect.h = static_cast<int>(ScreenSize.y);
		//�������� ��ġ�� �߽����� �簢���� ��ġ
		Rect.x = static_cast<int>(Owner->GetPosition().x - Rect.w / 2 + BGTexture.Offset.x);
		Rect.y = static_cast<int>(Owner->GetPosition().y - Rect.h / 2 + BGTexture.Offset.y);

		//�׸���
		SDL_RenderCopy(Renderer,
			BGTexture.Texture,
			nullptr,
			&Rect);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& Textures)
{
	int Count = 0;

	for (auto Texture : Textures)
	{
		BGTexture BGTextureData;
		BGTextureData.Texture = Texture;
		//�� �ؽ�ó�� �������� (ȭ���� �ʺ�(Width) * Count)
		BGTextureData.Offset.x = Count * ScreenSize.x;
		BGTextureData.Offset.y = 0;
		
		BGTextures.emplace_back(BGTextureData);
		Count++;
	}
}
