#include "Game.h"

Game::Game()
	: Window(nullptr)
	, Renderer(nullptr)
	, LeftPaddlePos(10.f, SCREEN_HEIGHT / 2.f)
	, RightPaddlePos(SCREEN_WIDTH - 10.f, SCREEN_HEIGHT / 2.f)
	, Balls()
	, TicksCount(0)
	, bIsRunning(true)
{
}

Game::~Game()
{
}

bool Game::Initialize()
{
	Balls.push_back(Ball(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f), Vector2(-200.f, 235.f)));
	Balls.push_back(Ball(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f), Vector2(200.f, 235.f)));

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

	LeftPaddleDir = 0;
	if (State[SDL_SCANCODE_W])
	{
		LeftPaddleDir -= 1;
	}
	if (State[SDL_SCANCODE_S])
	{
		LeftPaddleDir += 1;
	}

	RightPaddleDir = 0;
	if (State[SDL_SCANCODE_I])
	{
		RightPaddleDir -= 1;
	}
	if (State[SDL_SCANCODE_K])
	{
		RightPaddleDir += 1;
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

	if (LeftPaddleDir != 0)
	{
		LeftPaddlePos.y += LeftPaddleDir * 300.0f * DeltaTime;
		//�е��� ȭ���� ������� ����
		if (LeftPaddlePos.y < (PADDLE_HEIGHT / 2.0f + THICKNESS))
		{
			LeftPaddlePos.y = PADDLE_HEIGHT / 2.f + THICKNESS;
		}
		else if (LeftPaddlePos.y > (SCREEN_HEIGHT - PADDLE_HEIGHT / 2.0f - THICKNESS))
		{
			LeftPaddlePos.y = SCREEN_HEIGHT - PADDLE_HEIGHT / 2.f - THICKNESS;
		}
	}

	if (RightPaddleDir != 0)
	{
		RightPaddlePos.y += RightPaddleDir * 300.0f * DeltaTime;
		//�е��� ȭ���� ������� ����
		if (RightPaddlePos.y < (PADDLE_HEIGHT / 2.0f + THICKNESS))
		{
			RightPaddlePos.y = PADDLE_HEIGHT / 2.f + THICKNESS;
		}
		else if (RightPaddlePos.y > (SCREEN_HEIGHT - PADDLE_HEIGHT / 2.0f - THICKNESS))
		{
			RightPaddlePos.y = SCREEN_HEIGHT - PADDLE_HEIGHT / 2.f - THICKNESS;
		}
	}

	for (auto& Ball : Balls)
	{
		Ball.Position.x += Ball.Velocity.x * DeltaTime;
		Ball.Position.y += Ball.Velocity.y * DeltaTime;


		if (Ball.Position.y <= THICKNESS && Ball.Velocity.y < 0.f)
		{
			Ball.Velocity.y *= -1.f;
		}

		//�е�� ���� ���� �Ÿ� ���
		float LeftDiff = LeftPaddlePos.y - Ball.Position.y;
		float RightDiff = RightPaddlePos.y - Ball.Position.y;
		//����ϱ� ���ϵ��� �������� ����
		LeftDiff = fabs(LeftDiff);
		RightDiff = fabs(RightDiff);

		float HalfPaddleHeight = PADDLE_HEIGHT / 2.f;

		if (//Y ���̰� �е�ȿ� ������
			(LeftDiff <= HalfPaddleHeight) &&
			//�� ��ġ�� �е鿡 ������ X ���� �ȿ� �ְ�
			Ball.Position.x <= 25.f && Ball.Position.x >= 20.f &&
			//���� �������� ���� ���� ���
			Ball.Velocity.x < 0.f)
		{
			//�ݻ�
			Ball.Velocity.x *= -1.f;
		}
		
		if (//Y ���̰� �е�ȿ� ������
			(RightDiff <= HalfPaddleHeight) &&
			//�� ��ġ�� �е鿡 ������ X ���� �ȿ� �ְ�
			Ball.Position.x >= SCREEN_WIDTH-THICKNESS &&
			//���� ���������� ���� ���� ���
			Ball.Velocity.x > 0.f)
		{
			//�ݻ�
			Ball.Velocity.x *= -1.f;
		}

		//���� ȭ�� ������ �����ٸ�
		else if (Ball.Position.x <= 0.f || Ball.Position.x >= SCREEN_WIDTH)
		{
			//���� ����
			bIsRunning = false;
		}
		
		////���� ������ ���� �ε�����?
		//else if (Ball.Position.x >= (SCREEN_WIDTH - THICKNESS) && Ball.Velocity.x > 0.f)
		//{
		//	Ball.Velocity.x *= -1.f;
		//}
		
		//���� ���� ���� �ε�����?
		if (Ball.Position.y <= THICKNESS && Ball.Velocity.y < 0.f)
		{
			Ball.Velocity.y *= -1.f;
		}
		//���� �Ʒ��� ���� �ε�����?
		else if (Ball.Position.y >= (SCREEN_HEIGHT - THICKNESS) &&
			Ball.Velocity.y > 0.f)
		{
			Ball.Velocity.y *= -1.f;
		}
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

	//SDL_Rect RightWall{
	//SCREEN_WIDTH - THICKNESS,				//�»�� X
	//0,				//�»�� Y
	//SCREEN_WIDTH,	//����
	//SCREEN_HEIGHT		//����
	//};

	SDL_RenderFillRect(Renderer, &TopWall);
	SDL_RenderFillRect(Renderer, &BottomWall);
	//SDL_RenderFillRect(Renderer, &RightWall);
}

void Game::DrawPaddles()
{
	SDL_Rect LeftPaddle{
		static_cast<int>(LeftPaddlePos.x),
		static_cast<int>(LeftPaddlePos.y - PADDLE_HEIGHT / 2),
		THICKNESS,
		static_cast<int>(PADDLE_HEIGHT)
	};

	SDL_Rect RightPaddle{
		static_cast<int>(RightPaddlePos.x),
		static_cast<int>(RightPaddlePos.y - PADDLE_HEIGHT / 2),
		THICKNESS,
		static_cast<int>(PADDLE_HEIGHT)
	};

	SDL_RenderFillRect(Renderer, &LeftPaddle);
	SDL_RenderFillRect(Renderer, &RightPaddle);
}

void Game::DrawBalls()
{
	constexpr int CENTRIC = THICKNESS / 2;

	for (auto& Ball : Balls)
	{
		SDL_Rect BallRect
		{
			static_cast<int>(Ball.Position.x - CENTRIC),
			static_cast<int>(Ball.Position.y - CENTRIC),
			THICKNESS,
			THICKNESS,
		};

		SDL_RenderFillRect(Renderer, &BallRect);
	}
}
