#pragma once

#include "Actor.h"

class ActorComponent
{
public:
	//������Ʈ �������� �������� ������Ʈ�� �� ���� ���ŵȴ�.
	ActorComponent(Actor* InOwner, int InUpdateOrder = 100);
	virtual ~ActorComponent();

	//��Ÿ �ð����� �� ������Ʈ�� ������Ʈ
	virtual void Update(float DeltaTime);
	
	//������Ʈ �Է� ó��
	virtual void ProcessInput(const uint8_t* KeyState) {}

	inline int GetUpdateOrder() const { return UpdateOrder; }

protected:
	//������ ����
	Actor* Owner = nullptr;
	//������Ʈ�� ������Ʈ ����
	int UpdateOrder = 100;
};

