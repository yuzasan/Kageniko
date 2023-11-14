#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Title :public Task {
	//‰æ‘œ
	CImage m_img;
	//•¶Žš
	CFont m_title_text;
	int m_cnt;
	float m_time;
	bool m_isOn;

	enum class Step{
		Start,
		Mid,
		End,
	};
	Step m_step;

	void StepFirst();
	void StepSecond();
	void StepLast();
public:
	Title();
	~Title();
	void Update();
	void Render();
};