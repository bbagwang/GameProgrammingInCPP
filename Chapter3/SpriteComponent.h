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
	
	inline void SetDrawingState(bool bNewState) { bDrawing = bNewState; }
	inline bool GetDrawingState() const { return bDrawing; }

	inline int GetDrawOrder() const { return DrawOrder; }
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }

protected:
	bool bDrawing;

	//�׸� �ؽ�ó
	SDL_Texture* Texture;

	//ȭ�� �˰��򿡼� ����ϴ� �׸��� ����
	int DrawOrder;
	//�ؽ�ó�� �ʺ�
	int Width;
	//�ؽ�ó�� ����
	int Height;
};

