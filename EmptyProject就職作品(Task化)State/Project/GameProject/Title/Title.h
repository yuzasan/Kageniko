#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Title :public Task {
	//‰æ‘œ
	CImage m_img;
	//•¶Žš
	CFont m_title_text;
public:
	Title();
	~Title();
	void Update();
	void Render();
};