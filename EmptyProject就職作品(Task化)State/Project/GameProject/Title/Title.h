#pragma once
//#include "../Base/Base.h"
#include "../TaskSystem/Task.h"

class Title :public Task {
	//�摜
	CImage m_img;
	//����
	CFont m_title_text;
public:
	Title();
	~Title();
	void Update();
	void Render();
};