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
		"Game Programming in C++ (Chapter 1)",	//������ ����
		100,	//������ �˻�� X ��ǥ
		100,	//������ �»�� Y ��ǥ
		SCREEN_WIDTH,	//������ ����
		SCREEN_HEIGHT,	//������ ����
		0		//�÷��� (0�� �÷��� ����)
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

	//��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ����.
	//���� �� ������ ��ȯ
	float DeltaTime = (SDL_GetTicks() - TicksCount) / 1000.f;

	//���� �������� ���� ƽ���� ����
	TicksCount = SDL_GetTicks();

	//�ִ� ��Ÿ �ð� ������ ����
	if (DeltaTime > 0.05f)
	{
		DeltaTime = 0.05f;
	}

	if (PaddleDir != 0)
	{
		PaddlePos.y += PaddleDir * 300.0f * DeltaTime;
		//�е��� ȭ���� ������� ����
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

	//�е�� ���� ���� �Ÿ� ���
	float Diff = PaddlePos.y - BallPos.y;
	//����ϱ� ���ϵ��� �������� ����
	Diff = fabs(Diff);

	if (//Y ���̰� �е�ȿ� ������
		Diff <= PADDLE_HEIGHT / 2.f &&
		//�� ��ġ�� �е鿡 ������ X ���� �ȿ� �ְ�
		BallPos.x <= 25.f && BallPos.x >= 20.f &&
		//���� �������� ���� ���� ���
		BallVel.x < 0.f)
	{
		//�ݻ�
		BallVel.x *= -1.f;
	}
	//���� ȭ�� ������ �����ٸ�
	else if (BallPos.x <= 0.f)
	{
		//���� ����
		bIsRunning = false;
	}
	//���� ������ ���� �ε�����?
	else if (BallPos.x >= (SCREEN_WIDTH - THICKNESS) && BallVel.x > 0.f)
	{
		BallVel.x *= -1.f;
	}
	//���� ���� ���� �ε�����?
	if (BallPos.y <= THICKNESS && BallVel.y < 0.f)
	{
		BallVel.y *= -1.f;
	}
	//���� �Ʒ��� ���� �ε�����?
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

	//Back Buffer �ʱ�ȭ
	SDL_RenderClear(Renderer);

	DrawWalls();
	DrawPaddles();
	DrawBalls();

	SDL_RenderPresent(Renderer);
}

void Game::DrawWalls()
{
	//�� �׸���
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	SDL_Rect TopWall{
		0,				//�»�� X
		0,				//�»�� Y
		SCREEN_WIDTH,	//����
		THICKNESS		//����
	};

	SDL_Rect BottomWall{
	0,				//�»�� X
	SCREEN_HEIGHT - THICKNESS,//�»�� Y
	SCREEN_WIDTH,	//����
	THICKNESS		//����
	};

	SDL_Rect RightWall{
	SCREEN_WIDTH - THICKNESS,				//�»�� X
	0,				//�»�� Y
	SCREEN_WIDTH,	//����
	SCREEN_HEIGHT		//����
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
