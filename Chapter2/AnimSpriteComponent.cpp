#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* Owner, int DrawOrder)
	: SpriteComponent(Owner, DrawOrder)
	, AnimSetMap()
	, CurrentAnimSetIndex(0)
	, CurrentAnimSet(0, 0)
	, AnimTextures()
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
		CurrentFrame += (AnimFPS * DeltaTime);
		
		//�ִϸ��̼� �ؽ�ó ���� �ʰ��ϸ� ���� �������� �����Ѵ�
		if (CurrentAnimSet.bLoop)
		{
			if (CurrentFrame > CurrentAnimSet.End)
			{
				CurrentFrame = static_cast<float>(CurrentAnimSet.Start);
			}
		}
		else
		{
			if (CurrentFrame > CurrentAnimSet.End)
			{
				CurrentFrame = CurrentAnimSet.End;
			}
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
		AddAnimSet(-1, AnimSet(0, AnimTextures.size() - 1));

		//ù ������ �ؽ�ó�� �ʱ�ȭ�Ѵ�
		CurrentFrame = 0.f;
		SetTexture(AnimTextures[0]);
	}
}

void AnimSpriteComponent::AddAnimSet(int AnimSetKey, const AnimSet& NewAnimSet)
{
	AnimSetMap.emplace(AnimSetKey, NewAnimSet);
}

void AnimSpriteComponent::SetAnimSet(int AnimSetKey)
{
	auto FoundAnimSetIter = AnimSetMap.find(AnimSetKey);
	if (FoundAnimSetIter == AnimSetMap.end())
		return;

	CurrentAnimSet = FoundAnimSetIter->second;
	CurrentAnimSetIndex = FoundAnimSetIter->first;
	CurrentFrame = CurrentAnimSet.Start;
}

AnimSet AnimSpriteComponent::GetAnimSet(int AnimSetKey)
{
	auto FoundAnimSetIter = AnimSetMap.find(AnimSetKey);
	if (FoundAnimSetIter != AnimSetMap.end())
	{
		return FoundAnimSetIter->second;
	}

	return AnimSet(-1, -1);
}

void AnimSpriteComponent::SetAnimSetLoopState(int AnimSetKey, bool bNewLoopState)
{
	auto FoundAnimSetIter = AnimSetMap.find(AnimSetKey);
	if (FoundAnimSetIter != AnimSetMap.end())
	{
		FoundAnimSetIter->second.bLoop = bNewLoopState;
	}
}

void AnimSpriteComponent::SetLoop(bool bNewLoopState)
{
	SetAnimSetLoopState(CurrentAnimSetIndex, bNewLoopState);
}

bool AnimSpriteComponent::GetLoop() const
{
	auto FoundAnimSetIter = AnimSetMap.find(CurrentAnimSetIndex);
	if (FoundAnimSetIter != AnimSetMap.end())
	{
		return FoundAnimSetIter->second.bLoop;
	}

	return false;
}

bool AnimSpriteComponent::IsInvalidAnimSet(const AnimSet& CheckAnimSet) const
{
	return (CheckAnimSet.Start == -1 || CheckAnimSet.End == -1);
}
