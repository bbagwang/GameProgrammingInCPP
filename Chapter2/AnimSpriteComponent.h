#pragma once

#include "SpriteComponent.h"

struct AnimSet
{
public:
	AnimSet(int InStart, int InEnd, bool bInLoop = true)
		: Start(InStart), End(InEnd), bLoop(bInLoop) {}

	int Start;
	int End;
	bool bLoop;
};

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* Owner, int DrawOrder = 100);
	virtual ~AnimSpriteComponent() = default;

	//�ִϸ��̼��� �����Ӹ��� ���� (Component�� Update �޼ҵ带 �������̵�)
	void Update(float DeltaTime) override;
	//�ִϸ��̼ǿ� ���Ǵ� �ؽ�ó ����
	void SetAnimTextures(const std::vector<SDL_Texture*>& Textures);
	
	//�ִϸ��̼� FPS�� ��ų� �����ϴ� �Լ�
	inline float GetAnimFPS() const { return AnimFPS; }
	inline void SetAnimFPS(float FPS) { AnimFPS = FPS; }

	void AddAnimSet(int AnimSetKey, const AnimSet& NewAnimSet);
	void SetAnimSet(int AnimSetKey);
	AnimSet GetAnimSet(int AnimSetKey);
	int GetAnimSetIndex() { return CurrentAnimSetIndex; };
	void SetAnimSetLoopState(int AnimSetKey, bool bNewLoopState);
	void SetLoop(bool bNewLoopState);
	bool GetLoop() const;
	bool IsInvalidAnimSet(const AnimSet& CheckAnimSet) const;

private:
	std::unordered_map<int, AnimSet> AnimSetMap;

	int CurrentAnimSetIndex;
	AnimSet CurrentAnimSet;
	//�ִϸ��̼ǿ� ���Ǵ� ��� �ؽ�ó
	std::vector<SDL_Texture*> AnimTextures;
	//���� ������;
	float CurrentFrame;
	//�ִϸ��̼� ������ ����Ʈ
	float AnimFPS;
};

