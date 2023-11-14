#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Title :public Task {
	//‰æ‘œ
	CImage m_img;
	//•¶Žš
	CFont m_title_text;

	enum class Step{
		Start,
		Mid,
		End,
	};
	Step m_step;
public:
	Title();
	~Title();
	void Update();
	void Render();
};