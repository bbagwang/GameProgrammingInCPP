#pragma once

#include "SpriteComponent.h"

class BGSpriteComponent : public SpriteComponent
{
public:
	//����̹Ƿ� �⺻ �׸��� �������� ���� ������ ����
	BGSpriteComponent(Actor* InOwner, int InDrawOrder = 10);
	virtual ~BGSpriteComponent() = default;

	void Update(float DeltaTime) override;
	void Draw(SDL_Renderer* Renderer) override;

	//��濡 ���Ǵ� �ؽ�ó ����
	void SetBGTextures(const std::vector<SDL_Texture*>& Textures);

	//ȭ�� ũ�⸦ ��� �Լ�
	inline void SetScreenSize(const Vector2& Size) { ScreenSize = Size; }
	inline const Vector2& GetScreenSize() const { return ScreenSize; }
	
	//ȭ�� ��ũ�� �ӵ��� ���� / ��� 
	inline void SetScrollSpeed(float Speed) { ScrollSpeed = Speed; }
	inline float GetScrollSpeed() const { ScrollSpeed; }

private:
	//�� ��� �̹����� ��� �̹����� ������ ���� ĸ��ȭ�� ����ü
	struct BGTexture
	{
		SDL_Texture* Texture;
		Vector2 Offset;
	};

	std::vector<BGTexture> BGTextures;
	Vector2 ScreenSize;
	float ScrollSpeed;
};

