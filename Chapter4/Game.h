#pragma once

#include <vector>
#include <unordered_map>

#include "Math.h"

#include "SDL.h"

class Actor;
class Enemy;
class Grid;
class SpriteComponent;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

class Game
{
public:
	Game();
	virtual ~Game() = default;

	//게임 초기화
	bool Initialize();
	//게임이 끝나기 전까지 게임 루프를 실행
	void RunLoop();
	//게임 종료
	void Shutdown();

	void AddActor(Actor* InActor);
	void RemoveActor(Actor* InActor);

	void AddSprite(SpriteComponent* InSpriteComponent);
	void RemoveSprite(SpriteComponent* InSpriteComponent);

	SDL_Texture* GetTexture(const std::string& FileName);

	Grid* GetGrid() { return mGrid; }
	std::vector<Enemy*>& GetEnemies() { return mEnemies; }
	Enemy* GetNearestEnemy(const Vector2& pos);

private:
	//게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

private:
	//SDL이 생성한 윈도우
	SDL_Window* Window;
	//SDL이 생성한 렌더러
	SDL_Renderer* Renderer;

	Uint32 TicksCount;

	//텍스쳐 맵
	std::unordered_map<std::string, SDL_Texture*> Textures;

	//스프라이트 컴포넌트들
	std::vector<SpriteComponent*> Sprites;

	//업데이트중인 액터
	std::vector<Actor*> Actors;
	//업데이트 대기중인 액터
	std::vector<Actor*> PendingActors;

	std::vector<Enemy*> mEnemies;
	Grid* mGrid;
	float mNextEnemy;

	//게임이 계속 실행되어야 하는지 판단
	bool bIsRunning;
	//액터를 갱신중인지
	bool bUpdatingActors;
};
