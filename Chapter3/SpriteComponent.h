#pragma once

#include "ActorComponent.h"

#include "SDL.h"

class SpriteComponent : public ActorComponent
{
public:
	//�׸��� �������� �������� �� ���ʿ��� �׸���
	SpriteComponent(Actor* InOwner, int InDrawOrder = 100);
	virtual ~SpriteComponent() override;

	virtual void Draw(SDL_Renderer* Renderer);
	virtual void SetTexture(SDL_Texture* InTexture);

	inline int GetDrawOrder() const { return DrawOrder; }
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }

protected:
	//�׸� �ؽ�ó
	SDL_Texture* Texture;

	//ȭ�� �˰��򿡼� ����ϴ� �׸��� ����
	int DrawOrder;
	//�ؽ�ó�� �ʺ�
	int Width;
	//�ؽ�ó�� ����
	int Height;
};

