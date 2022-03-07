#pragma once

#include "SpriteComponent.h"

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(Actor* InOwner, int DrawOrder = 10);
	virtual ~TileMapComponent() = default;

	virtual void Draw(SDL_Renderer* Renderer) override;
	virtual void SetTexture(SDL_Texture* InTexture) override;

	//ȭ�� ũ�⸦ ��� �Լ�
	inline void SetScreenSize(const Vector2& Size) { ScreenSize = Size; }
	inline const Vector2& GetScreenSize() const { return ScreenSize; }

	void LoadTileMap(const std::string CSVFileString);

private:
	SDL_Texture* TileTexture;
	Vector2 ScreenSize;
	int TileMapWidth;
	int TileMapHeight;
	std::vector<int> TileMap;
};

