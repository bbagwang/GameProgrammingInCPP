#pragma once
#include "PCH.h"

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

	//���� �е� ��ġ
	Vector2 LeftPaddlePos;
	//������ �е� ��ġ
	Vector2 RightPaddlePos;

	//�� �迭
	std::vector<Ball> Balls;
	
	Uint32 TicksCount;

	//���� �е� ����
	int LeftPaddleDir = 0;
	//������ �е� ����
	int RightPaddleDir = 0;

	//������ ��� ����Ǿ�� �ϴ��� �Ǵ�
	bool bIsRunning;
};

