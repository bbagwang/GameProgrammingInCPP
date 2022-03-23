#pragma once

#include <vector>
#include <unordered_map>

#include "Math.h"

#include "SDL.h"

#include "BoardState.h"

class Actor;
class SpriteComponent;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

class Game
{
public:
	Game();
	virtual ~Game() = default;

	//���� �ʱ�ȭ
	bool Initialize();
	//������ ������ ������ ���� ������ ����
	void RunLoop();
	//���� ����
	void Shutdown();

	void AddActor(Actor* InActor);
	void RemoveActor(Actor* InActor);

	void AddSprite(SpriteComponent* InSpriteComponent);
	void RemoveSprite(SpriteComponent* InSpriteComponent);

	SDL_Texture* GetTexture(const std::string& FileName);
	void DrawTexture(SDL_Texture* texture, const Vector2& pos, const Vector2& size);

private:
	//���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

private:
	//SDL�� ������ ������
	SDL_Window* Window;
	//SDL�� ������ ������
	SDL_Renderer* Renderer;

	Uint32 TicksCount;

	//�ؽ��� ��
	std::unordered_map<std::string, SDL_Texture*> Textures;

	//��������Ʈ ������Ʈ��
	std::vector<SpriteComponent*> Sprites;

	//������Ʈ���� ����
	std::vector<Actor*> Actors;
	//������Ʈ ������� ����
	std::vector<Actor*> PendingActors;

	//������ ��� ����Ǿ�� �ϴ��� �Ǵ�
	bool bIsRunning;
	//���͸� ����������
	bool bUpdatingActors;

	BoardState mBoardState;
};