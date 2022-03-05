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
		//������ ����Ʈ�� ��Ÿ �ð��� ������� ���� ������ ����
		CurrentFrame += AnimFPS * DeltaTime;
		
		//�ִԿ��Ӽ� �ؽ�ó ���� �ʰ��ϸ� ���� �������� �����Ѵ�
		while (CurrentFrame >= AnimTextures.size())
		{
			CurrentFrame -= AnimTextures.size();
		}

		//���� �ؽ�ó�� ����
		SetTexture(AnimTextures[static_cast<int>(CurrentFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& Textures)
{
	AnimTextures = Textures;
	if (AnimTextures.size() > 0)
	{
		//ù ������ �ؽ�ó�� �ʱ�ȭ�Ѵ�
		CurrentFrame = 0.f;
		SetTexture(AnimTextures[0]);
	}
}
