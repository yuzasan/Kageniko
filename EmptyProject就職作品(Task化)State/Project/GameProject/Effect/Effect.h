#pragma once
#include "../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class Effect :public Task {
private:
	CModelObj m_model;
	bool m_builbord;
	float m_scale;
	int m_time;
public:
	Effect(const char* model, const CVector3D& pos, float scale, bool builbord, int time);
	~Effect();
	void Update();
	void Render();
};

class Effect2D :public Task {
private:
	CImage m_img;
public:
	Effect2D(const char* img, const CVector2D& pos);
	~Effect2D();
	void Update();
	void LateRender2D();
};