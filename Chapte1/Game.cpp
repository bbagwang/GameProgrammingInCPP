#include "Game.h"

#include <SDL.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int THICKNESS = 15;
const int PADDLE_HEIGHT = 100;

Game::Game()
	: Window(nullptr)
	, Renderer(nullptr)
	, PaddlePos(10.f, SCREEN_HEIGHT / 2.f)
	, BallPos(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f)
	, BallVel(-200.f, 235.f)
	, TicksCount(0)
	, bIsRunning(true)
{
}

Game::~Game()
{
}

bool Game::Initialize()
{
	int SdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (SdlResult != 0)
	{
		SDL_Log("Unable to Initialize SDL : %s", SDL_GetError());
		return false;
	}

	Window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)",	//윈도우 제목
		100,	//윈도우 죄상단 X 좌표
		100,	//윈도우 좌상단 Y 좌표
		SCREEN_WIDTH,	//윈도우 넓이
		SCREEN_HEIGHT,	//윈도우 높이
		0		//플래그 (0은 플래그 없음)
	);

	if (!Window)
	{
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	Renderer = SDL_CreateRenderer(
		Window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!Renderer)
	{
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::RunLoop()
{
	while (bIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			bIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* State = SDL_GetKeyboardState(NULL);
	if (State[SDL_SCANCODE_ESCAPE])
	{
		bIsRunning = false;
	}

	PaddleDir = 0;
	if (State[SDL_SCANCODE_W])
	{
		PaddleDir -= 1;
	}
	if (State[SDL_SCANCODE_S])
	{
		PaddleDir += 1;
	}

}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16.f));

	//델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차다.
	//차를 초 단위로 변환
	float DeltaTime = (SDL_GetTicks() - TicksCount) / 1000.f;

	//다음 프레임을 위해 틱값을 갱신
	TicksCount = SDL_GetTicks();

	//최대 델타 시간 값으로 고정
	if (DeltaTime > 0.05f)
	{
		DeltaTime = 0.05f;
	}

	if (PaddleDir != 0)
	{
		PaddlePos.y += PaddleDir * 300.0f * DeltaTime;
		//패들이 화면을 벗어나는지 검증
		if (PaddlePos.y < (PADDLE_HEIGHT / 2.0f + THICKNESS))
		{
			PaddlePos.y = PADDLE_HEIGHT / 2.f + THICKNESS;
		}
		else if (PaddlePos.y > (SCREEN_HEIGHT - PADDLE_HEIGHT / 2.0f - THICKNESS))
		{
			PaddlePos.y = SCREEN_HEIGHT - PADDLE_HEIGHT / 2.f - THICKNESS;
		}
	}

	BallPos.x += BallVel.x * DeltaTime;
	BallPos.y += BallVel.y * DeltaTime;

	if (BallPos.y <= THICKNESS && BallVel.y < 0.f)
	{
		BallVel.y *= -1.f;
	}

	//패들과 공의 수직 거리 계산
	float Diff = PaddlePos.y - BallPos.y;
	//계산하기 편하도록 절댓값으로 보정
	Diff = fabs(Diff);

	if (//Y 차이가 패들안에 들어오고
		Diff <= PADDLE_HEIGHT / 2.f &&
		//공 위치가 패들에 적절한 X 차이 안에 있고
		BallPos.x <= 25.f && BallPos.x >= 20.f &&
		//공이 왼쪽으로 오고 있을 경우
		BallVel.x < 0.f)
	{
		//반사
		BallVel.x *= -1.f;
	}
	//볼이 화면 밖으로 나갔다면
	else if (BallPos.x <= 0.f)
	{
		//게임 종료
		bIsRunning = false;
	}
	//공이 오른쪽 벽에 부딪혔나?
	else if (BallPos.x >= (SCREEN_WIDTH - THICKNESS) && BallVel.x > 0.f)
	{
		BallVel.x *= -1.f;
	}
	//공이 위쪽 벽에 부딪혔나?
	if (BallPos.y <= THICKNESS && BallVel.y < 0.f)
	{
		BallVel.y *= -1.f;
	}
	//공이 아래쪽 벽에 부딪혔나?
	else if (BallPos.y >= (SCREEN_HEIGHT - THICKNESS) &&
		BallVel.y > 0.f)
	{
		BallVel.y *= -1.f;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		Renderer,
		0,		//R
		0,		//G
		255,	//B
		255		//A
	);

	//Back Buffer 초기화
	SDL_RenderClear(Renderer);

	DrawWalls();
	DrawPaddles();
	DrawBalls();

	SDL_RenderPresent(Renderer);
}

void Game::DrawWalls()
{
	//벽 그리기
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	SDL_Rect TopWall{
		0,				//좌상단 X
		0,				//좌상단 Y
		SCREEN_WIDTH,	//넓이
		THICKNESS		//높이
	};

	SDL_Rect BottomWall{
	0,				//좌상단 X
	SCREEN_HEIGHT - THICKNESS,//좌상단 Y
	SCREEN_WIDTH,	//넓이
	THICKNESS		//높이
	};

	SDL_Rect RightWall{
	SCREEN_WIDTH - THICKNESS,				//좌상단 X
	0,				//좌상단 Y
	SCREEN_WIDTH,	//넓이
	SCREEN_HEIGHT		//높이
	};

	SDL_RenderFillRect(Renderer, &TopWall);
	SDL_RenderFillRect(Renderer, &BottomWall);
	SDL_RenderFillRect(Renderer, &RightWall);
}

void Game::DrawPaddles()
{
	SDL_Rect Paddle{
		static_cast<int>(PaddlePos.x),
		static_cast<int>(PaddlePos.y - PADDLE_HEIGHT / 2),
		THICKNESS,
		static_cast<int>(PADDLE_HEIGHT)
	};

	SDL_RenderFillRect(Renderer, &Paddle);
}

void Game::DrawBalls()
{
	constexpr int CENTRIC = THICKNESS / 2;

	SDL_Rect Ball
	{
		static_cast<int>(BallPos.x - CENTRIC),
		static_cast<int>(BallPos.y - CENTRIC),
		THICKNESS,
		THICKNESS,
	};

	SDL_RenderFillRect(Renderer, &Ball);
}
