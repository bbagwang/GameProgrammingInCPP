#pragma once

#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* Owner, int DrawOrder = 100);
	virtual ~AnimSpriteComponent() = default;

	//애니메이션을 프레임마다 갱신 (Component의 Update 메소드를 오버라이드)
	void Update(float DeltaTime) override;
	//애니메이션에 사용되는 텍스처 설정
	void SetAnimTextures(const std::vector<SDL_Texture*>& Textures);
	
	//애니메이션 FPS를 얻거나 설정하는 함수
	inline float GetAnimFPS() const { return AnimFPS; }
	inline void SetAnimFPS(float FPS) { AnimFPS = FPS; }

private:
	//애니메이션에 사용되는 모든 텍스처
	std::vector<SDL_Texture*> AnimTextures;
	//현재 프레임;
	float CurrentFrame;
	//애니메이션 프레임 레이트
	float AnimFPS;
};

