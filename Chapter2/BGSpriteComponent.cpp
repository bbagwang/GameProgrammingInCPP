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
		//X 오프셋 값을 갱신
		BGTexture.Offset.x += ScrollSpeed * DeltaTime;
		//이 텍스처가 화면을 완전히 벗어나면 오프셋 값을
		//마지막 배경 텍스처의 오른쪽 위치로 초기화한다.
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

	//각각의 배경 텍스쳐를 그린다.
	for (auto& BGTexture : BGTextures)
	{
		SDL_Rect Rect;
		//화면 크기 설정
		Rect.w = static_cast<int>(ScreenSize.x);
		Rect.h = static_cast<int>(ScreenSize.y);
		//소유자의 위치를 중심으로 사각형을 배치
		Rect.x = static_cast<int>(Owner->GetPosition().x - Rect.w / 2 + BGTexture.Offset.x);
		Rect.y = static_cast<int>(Owner->GetPosition().y - Rect.h / 2 + BGTexture.Offset.y);

		//그리기
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
		//각 텍스처의 오프셋은 (화면의 너비(Width) * Count)
		BGTextureData.Offset.x = Count * ScreenSize.x;
		BGTextureData.Offset.y = 0;
		
		BGTextures.emplace_back(BGTextureData);
		Count++;
	}
}
