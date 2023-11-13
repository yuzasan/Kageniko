#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"
#include "../Game/Object/ObjectBase.h"

class Game : public Task {
public:
	Game();
	~Game();
	void Update();
};

//�Ó]
class BlackOut : public Task {
public:
	int m_gauge;	//�c��
	int m_num;		//�ԍ�
	float m_alpha;
	BlackOut();
	~BlackOut();
	void LateUpdate();
};