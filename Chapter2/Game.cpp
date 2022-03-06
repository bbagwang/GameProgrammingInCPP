#include "Game.h"

#include "SDL_image.h"

#include "Actor.h"
#include "ActorState.h"
#include "BGSpriteComponent.h"
#include "TileMapComponent.h"
#include "Ship.h"
#include "Character.h"

Game::Game()
	: Window(nullptr)
	, Renderer(nullptr)
	, TicksCount(0)
	, Textures()
	, Sprites()
	, Actors()
	, PendingActors()
	, Player(nullptr)
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
		"Game Programming in C++ (Chapter 2)",	//윈도우 제목
		100,	//윈도우 죄상단 X 좌표
		100,	//윈도우 좌상단 Y 좌표
		SCREEN_WIDTH,	//윈도우 넓이
		SCREEN_HEIGHT,	//윈도우 높이
		0);		//플래그 (0은 플래그 없음)

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
	//액터를 갱신하는 중이라면 PendingActors에 액터를 추가
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
			//RemoveSingleSwap 느낌
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

	//정렬된 벡터에서 삽입해야 할 위치를 찾는다.
	// 자신보다 그리기 순서값이 큰 최초 요소
	int DrawOrder = InSpriteComponent->GetDrawOrder();
	auto Iter = Sprites.begin();
	for (; Iter != Sprites.end(); ++Iter)
	{
		if (DrawOrder < (*Iter)->GetDrawOrder())
		{
			break;
		}
	}

	//반복자 위치 앞에 요소를 삽입한다.
	Sprites.insert(Iter, InSpriteComponent);
}

void Game::RemoveSprite(SpriteComponent* InSpriteComponent)
{
	//Swap을 하면 정렬이 망가지므로 Erase 사용
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

	const Uint8* State = SDL_GetKeyboardState(NULL);
	if (State[SDL_SCANCODE_ESCAPE])
	{
		bIsRunning = false;
	}

	//플레이어 입력
	Player->ProcessKeyboard(State);
}

void Game::UpdateGame()
{
	//델타 타임 계산해보고 마지막 프레임으로부터 16ms 가 지날때 까지 기다린다.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16.f));

	//델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차다.
	//차를 초 단위로 변환
	float DeltaTime = (SDL_GetTicks() - TicksCount) / 1000.f;

	//최대 델타 시간 값으로 고정
	if (DeltaTime > 0.05f)
	{
		DeltaTime = 0.05f;
	}
	
	//다음 프레임을 위해 틱값을 갱신
	TicksCount = SDL_GetTicks();

	//모든 액터를 갱신
	bUpdatingActors = true;
	for (auto Actor : Actors)
	{
		Actor->Update(DeltaTime);
	}
	bUpdatingActors = false;

	//대기 중인 액터를 Actors로 이동
	for (auto PendingActor : PendingActors)
	{
		Actors.emplace_back(PendingActor);
	}
	PendingActors.clear();

	//죽은 액터를 임시 벡터에 추가
	std::vector<Actor*> DeadActors;
	for (auto Actor : Actors)
	{
		if (Actor->GetState() == ActorState::Dead)
		{
			DeadActors.emplace_back(Actor);
		}
	}

	//죽은 액터 제거
	for (auto DeadActor : DeadActors)
	{
		delete DeadActor;
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

	//Sprite 그리기
	for (auto Sprite : Sprites)
	{
		Sprite->Draw(Renderer);
	}

	SDL_RenderPresent(Renderer);
}

void Game::LoadData()
{
	//플레이어 Ship 액터 생성
	Player = new Character(this);
	Player->SetPosition(Vector2(100.0f, SCREEN_HEIGHT-50.f));
	Player->SetScale(1.0f);

	//타일맵 액터 생성
	Actor* TileActor = new Actor(this);
	TileActor->SetPosition(Vector2(0.f,0.f));

	TileMapComponent* TMC = new TileMapComponent(TileActor);
	TMC->SetScreenSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	TMC->SetTexture(GetTexture("Assets/Tiles.png"));
	TMC->LoadTileMap("Assets/MapLayer2.csv");
}

void Game::UnloadData()
{
	//~Actor 함수가 RemoveActor를 호출하므로 다른 스타일의 루프를 사용하자	
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
	//텍스쳐가 이미 맵에 있는지 체크
	auto Iter = Textures.find(FileName);
	if (Iter != Textures.end())
	{
		return Iter->second;
	}
	
	//파일로부터 로딩
	SDL_Surface* Surface = IMG_Load(FileName.c_str());
	if (!Surface)
	{
		SDL_Log("Failed to load texture file : %s", FileName);
		return nullptr;
	}
	
	//텍스쳐 생성
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
