#pragma once
#include "PCH.h"

class Game
{
public:
	Game();
	virtual ~Game();

	//게임 초기화
	bool Initialize();
	//게임이 끝나기 전까지 게임 루프를 실행
	void RunLoop();
	//게임 종료
	void Shutdown();

private:
	//게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void DrawWalls();
	void DrawPaddles();
	void DrawBalls();

private:
	//SDL이 생성한 윈도우
	SDL_Window* Window;
	//SDL이 생성한 렌더러
	SDL_Renderer* Renderer;

	//왼쪽 패들 위치
	Vector2 LeftPaddlePos;
	//오른쪽 패들 위치
	Vector2 RightPaddlePos;

	//공 배열
	std::vector<Ball> Balls;
	
	Uint32 TicksCount;

	//왼쪽 패들 방향
	int LeftPaddleDir = 0;
	//오른쪽 패들 방향
	int RightPaddleDir = 0;

	//게임이 계속 실행되어야 하는지 판단
	bool bIsRunning;
};

