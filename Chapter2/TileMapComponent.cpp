#include "TileMapComponent.h"

#include <fstream>
#include <sstream>
#include <string>

TileMapComponent::TileMapComponent(Actor* InOwner, int DrawOrder)
	: SpriteComponent(InOwner, DrawOrder)
	, TileTexture(nullptr)
	, ScreenSize(0,0)
	, TileMap()
{

}

void TileMapComponent::Draw(SDL_Renderer* Renderer)
{
	if (!Renderer)
		return;

	static const int RECT_SIZE = 32;
	static const int WIDTH_LENGTH = 32;
	static const int HEIGHT_LENGTH = 24;
	static const int TILE_WIDTH_LENGTH = 8;
	static const int TILE_HEIGHT_LENGTH = 24;

	int OffsetX = 0;
	int OffsetY = 0;

	//각각의 배경 텍스쳐를 그린다.
	for (int Y = 0; Y < HEIGHT_LENGTH; Y++)
	{
		for (int X = 0; X < WIDTH_LENGTH; X++)
		{
			int TileIndex = TileMap[Y + X];

			if (TileIndex == -1)
			{
				OffsetX += RECT_SIZE;
				continue;
			}

			int TileY = TileIndex / TILE_WIDTH_LENGTH;
			int TileX = TileIndex % TILE_WIDTH_LENGTH;

			SDL_Rect SourceRect;
			//화면 크기 설정
			SourceRect.w = static_cast<int>(RECT_SIZE);
			SourceRect.h = static_cast<int>(RECT_SIZE);
			//소유자의 위치를 중심으로 사각형을 배치
			int SRC_X = TileX * RECT_SIZE;
			int SRC_Y = TileY * RECT_SIZE;
			SourceRect.x = static_cast<int>(SRC_X);
			SourceRect.y = static_cast<int>(SRC_Y);

			SDL_Rect TargetRect;
			TargetRect.w = static_cast<int>(RECT_SIZE);
			TargetRect.h = static_cast<int>(RECT_SIZE);
			TargetRect.x = static_cast<int>(OffsetX);
			TargetRect.y = static_cast<int>(OffsetY);

			//그리기
			SDL_RenderCopyEx(Renderer,
				TileTexture,
				&SourceRect,
				&TargetRect,
				0,
				nullptr,
				SDL_FLIP_NONE);

			OffsetX += RECT_SIZE;
		}

		OffsetX = 0;
		OffsetY += RECT_SIZE;
	}
}

void TileMapComponent::SetTexture(SDL_Texture* InTexture)
{
	if (!InTexture)
		return;

	TileTexture = InTexture;

	//int Width, Height;
	//SDL_QueryTexture(InTexture, nullptr, nullptr, &Width, &Height);

	//static const int TileSize = 32;

	//int WidthLength = Width / 32;
	//int HeightLength = Height / 32;
	//
	//for (int HeightCount = 0; HeightCount < HeightLength; HeightCount++)
	//{
	//	for (int WidthCount = 0; WidthCount < WidthLength; WidthCount++)
	//	{
	//		
	//	}
	//}
}

void TileMapComponent::LoadTileMap(const std::string CSVFileString)
{
	//32 X 24

	std::fstream FileStream;
	std::string Buffer;
	FileStream.open(CSVFileString, std::ios::in);

	while (!FileStream.eof())
	{
		std::getline(FileStream, Buffer, ',');
		std::stringstream Converter(Buffer);

		int temp = 0;
		Converter >> temp;

		TileMap.push_back(temp);
	}

	FileStream.close();
}
