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

//à√ì]
class BlackOut : public Task {
public:
	int m_gauge;	//ècïù
	int m_num;		//î‘çÜ
	float m_alpha;
	BlackOut();
	~BlackOut();
	void LateUpdate();
};