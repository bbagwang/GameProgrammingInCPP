#pragma once

#include "SpriteComponent.h"

class BGSpriteComponent : public SpriteComponent
{
public:
	//배경이므로 기본 그리기 순서값을 낮은 값으로 설정
	BGSpriteComponent(Actor* InOwner, int InDrawOrder = 10);
	virtual ~BGSpriteComponent() = default;

	void Update(float DeltaTime) override;
	void Draw(SDL_Renderer* Renderer) override;

	//배경에 사용되는 텍스처 설정
	void SetBGTextures(const std::vector<SDL_Texture*>& Textures);

	//화면 크기를 얻는 함수
	inline void SetScreenSize(const Vector2& Size) { ScreenSize = Size; }
	inline const Vector2& GetScreenSize() const { return ScreenSize; }
	
	//화면 스크롤 속도값 설정 / 얻기 
	inline void SetScrollSpeed(float Speed) { ScrollSpeed = Speed; }
	inline float GetScrollSpeed() const { ScrollSpeed; }

private:
	//각 배경 이미지와 배경 이미지의 오프셋 값을 캡슐화한 구조체
	struct BGTexture
	{
		SDL_Texture* Texture;
		Vector2 Offset;
	};

	std::vector<BGTexture> BGTextures;
	Vector2 ScreenSize;
	float ScrollSpeed;
};

