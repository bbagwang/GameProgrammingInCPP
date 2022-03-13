#include "Game.h"

#include "SDL_image.h"

#include "Actor.h"
#include "ActorState.h"
#include "Asteroid.h"
#include "Random.h"
#include "Ship.h"
#include "SpriteComponent.h"

Game::Game()
	: Window(nullptr)
	, Renderer(nullptr)
	, TicksCount(0)
	, Textures()
	, Sprites()
	, Actors()
	, PendingActors()
	, Player(nullptr)
	, Asteroids()
	, bIsRunning(true)
	, bUpdatingActors(false)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to Initialize SDL : %s", SDL_GetError());
		return false;
	}

	Window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 2)",	//������ ����
		100,	//������ �˻�� X ��ǥ
		100,	//������ �»�� Y ��ǥ
		SCREEN_WIDTH,	//������ ����
		SCREEN_HEIGHT,	//������ ����
		0);		//�÷��� (0�� �÷��� ����)

	if (!Window)
	{
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	Renderer = SDL_CreateRenderer(
		Window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!Renderer)
	{
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Failed to initialize SDL_Image : %s", SDL_GetError());
	}

	Random::Init();

	LoadData();

	TicksCount = SDL_GetTicks();

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
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Game::AddActor(Actor* InActor)
{
	//���͸� �����ϴ� ���̶�� PendingActors�� ���͸� �߰�
	if (bUpdatingActors)
	{
		PendingActors.emplace_back(InActor);
	}
	else
	{
		Actors.emplace_back(InActor);
	}
}

void Game::RemoveActor(Actor* InActor)
{
	{
		auto Iter = std::find(PendingActors.begin(), PendingActors.end(), InActor);
		if (Iter != PendingActors.end())
		{
			//RemoveSingleSwap ����
			std::iter_swap(Iter, PendingActors.end() - 1);
			PendingActors.pop_back();
			return;
		}
	}

	{
		auto Iter = std::find(Actors.begin(), Actors.end(), InActor);
		if (Iter != Actors.end())
		{
			std::iter_swap(Iter, Actors.end() - 1);
			Actors.pop_back();
			return;
		}
	}
}

void Game::AddSprite(SpriteComponent* InSpriteComponent)
{
	if (!InSpriteComponent)
		return;

	//���ĵ� ���Ϳ��� �����ؾ� �� ��ġ�� ã�´�.
	// �ڽź��� �׸��� �������� ū ���� ���
	int DrawOrder = InSpriteComponent->GetDrawOrder();
	auto Iter = Sprites.begin();
	for (; Iter != Sprites.end(); ++Iter)
	{
		if (DrawOrder < (*Iter)->GetDrawOrder())
		{
			break;
		}
	}

	//�ݺ��� ��ġ �տ� ��Ҹ� �����Ѵ�.
	Sprites.insert(Iter, InSpriteComponent);
}

void Game::RemoveSprite(SpriteComponent* InSpriteComponent)
{
	//Swap�� �ϸ� ������ �������Ƿ� Erase ���
	auto Iter = std::find(Sprites.begin(), Sprites.end(), InSpriteComponent);
	if (Iter != Sprites.end())
	{
		Sprites.erase(Iter);
	}
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

	const Uint8* KeyState = SDL_GetKeyboardState(NULL);
	if (KeyState[SDL_SCANCODE_ESCAPE])
	{
		bIsRunning = false;
	}

	bUpdatingActors = true;
	for (auto Actor : Actors)
	{
		Actor->ProcessInput(KeyState);
	}
	bUpdatingActors = false;
}

void Game::UpdateGame()
{
	//��Ÿ Ÿ�� ����غ��� ������ ���������κ��� 16ms �� ������ ���� ��ٸ���.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16.f));

	//��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ����.
	//���� �� ������ ��ȯ
	float DeltaTime = (SDL_GetTicks() - TicksCount) / 1000.f;

	//�ִ� ��Ÿ �ð� ������ ����
	if (DeltaTime > 0.05f)
	{
		DeltaTime = 0.05f;
	}

	//���� �������� ���� ƽ���� ����
	TicksCount = SDL_GetTicks();

	//��� ���͸� ����
	bUpdatingActors = true;
	for (auto Actor : Actors)
	{
		Actor->Update(DeltaTime);
	}
	bUpdatingActors = false;

	//��� ���� ���͸� Actors�� �̵�
	for (auto PendingActor : PendingActors)
	{
		Actors.emplace_back(PendingActor);
	}
	PendingActors.clear();

	//���� ���͸� �ӽ� ���Ϳ� �߰�
	std::vector<Actor*> DeadActors;
	for (auto Actor : Actors)
	{
		if (Actor->GetState() == ActorState::Dead)
		{
			DeadActors.emplace_back(Actor);
		}
	}

	//���� ���� ����
	for (auto DeadActor : DeadActors)
	{
		delete DeadActor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(Renderer, 220, 220, 220, 255);

	//Back Buffer �ʱ�ȭ
	SDL_RenderClear(Renderer);

	//Sprite �׸���
	for (auto Sprite : Sprites)
	{
		Sprite->Draw(Renderer);
	}

	SDL_RenderPresent(Renderer);
}

void Game::LoadData()
{
	//�÷��̾� Ship ���� ����
	Player = new Ship(this);
	Player->SetPosition(Vector2(512.f, 384.f));
	Player->SetRotation(Math::PiOver2);

	//� ����
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	//~Actor �Լ��� RemoveActor�� ȣ���ϹǷ� �ٸ� ��Ÿ���� ������ �������	
	while (!Actors.empty())
	{
		delete Actors.back();
	}

	// Destroy textures
	for (auto Texture : Textures)
	{
		SDL_DestroyTexture(Texture.second);
	}

	Textures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& FileName)
{
	//�ؽ��İ� �̹� �ʿ� �ִ��� üũ
	auto Iter = Textures.find(FileName);
	if (Iter != Textures.end())
	{
		return Iter->second;
	}

	//���Ϸκ��� �ε�
	SDL_Surface* Surface = IMG_Load(FileName.c_str());
	if (!Surface)
	{
		SDL_Log("Failed to load texture file : %s", FileName);
		return nullptr;
	}

	//�ؽ��� ����
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
	SDL_FreeSurface(Surface);
	if (!Texture)
	{
		SDL_Log("Failed to convert surface to texture for : %s", FileName);
		return nullptr;
	}

	Textures.emplace(FileName, Texture);

	return Texture;
}

void Game::AddAsteroid(Asteroid* InAsetroid)
{
	Asteroids.emplace_back(InAsetroid);
}

void Game::RemoveAsteroid(Asteroid* InAsetroid)
{
	auto FindIter = std::find(Asteroids.begin(), Asteroids.end(), InAsetroid);

	if (FindIter != Asteroids.end())
	{
		Asteroids.erase(FindIter);
	}
}