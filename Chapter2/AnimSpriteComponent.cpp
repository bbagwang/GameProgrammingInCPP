#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* Owner, int DrawOrder)
	: SpriteComponent(Owner, DrawOrder)
	, CurrentFrame(0.f)
	, AnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float DeltaTime)
{
	SpriteComponent::Update(DeltaTime);

	if (AnimTextures.size() > 0)
	{
		//프레임 레이트와 델타 시간을 기반으로 현재 프레임 갱신
		CurrentFrame += AnimFPS * DeltaTime;
		
		//애님에ㅣ션 텍스처 수를 초과하면 현재 프레임을 래핑한다
		while (CurrentFrame >= AnimTextures.size())
		{
			CurrentFrame -= AnimTextures.size();
		}

		//현재 텍스처를 설정
		SetTexture(AnimTextures[static_cast<int>(CurrentFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& Textures)
{
	AnimTextures = Textures;
	if (AnimTextures.size() > 0)
	{
		//첫 프레임 텍스처로 초기화한다
		CurrentFrame = 0.f;
		SetTexture(AnimTextures[0]);
	}
}
