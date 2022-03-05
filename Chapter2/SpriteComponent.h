#pragma once

#include "ActorComponent.h"

#include "SDL.h"

class SpriteComponent : public ActorComponent
{
public:
	//그리기 순서값이 낮을수록 더 뒤쪽에서 그린다
	SpriteComponent(Actor* InOwner, int InDrawOrder = 100);
	virtual ~SpriteComponent() override;

	virtual void Draw(SDL_Renderer* Renderer);
	virtual void SetTexture(SDL_Texture* InTexture);

	inline int GetDrawOrder() const { return DrawOrder; }
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }

protected:
	//그릴 텍스처
	SDL_Texture* Texture;

	//화가 알고리즘에서 사용하는 그리기 순서
	int DrawOrder;
	//텍스처의 너비
	int Width;
	//텍스처의 높이
	int Height;
};

