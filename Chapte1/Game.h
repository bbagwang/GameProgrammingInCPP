#pragma once

#include "Vector2.h"

#include "SDL.h"

class Game
{
public:
	Game();
	virtual ~Game();

	//���� �ʱ�ȭ
	bool Initialize();
	//������ ������ ������ ���� ������ ����
	void RunLoop();
	//���� ����
	void Shutdown();

private:
	//���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void DrawWalls();
	void DrawPaddles();
	void DrawBalls();

private:
	//SDL�� ������ ������
	SDL_Window* Window;
	//SDL�� ������ ������
	SDL_Renderer* Renderer;

	//�е� ��ġ
	Vector2 PaddlePos;
	//�� ��ġ
	Vector2 BallPos;
	//�� �ӵ�
	Vector2 BallVel;
	
	Uint32 TicksCount;

	int PaddleDir = 0;

	//������ ��� ����Ǿ�� �ϴ��� �Ǵ�
	bool bIsRunning;
};

