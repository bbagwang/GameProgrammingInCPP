#pragma once

#include "SpriteComponent.h"

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

private:
	//�ִϸ��̼ǿ� ���Ǵ� ��� �ؽ�ó
	std::vector<SDL_Texture*> AnimTextures;
	//���� ������;
	float CurrentFrame;
	//�ִϸ��̼� ������ ����Ʈ
	float AnimFPS;
};

