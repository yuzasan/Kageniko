#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class Fade : public Task {
private:
	Fade();
	~Fade();
	static Fade* Instance();
	static Fade* ms_instance;
public:
	bool m_isFadeOut;	//フェードアウトor
	bool m_isFade;		//フェード中
	float m_alpha;			//アルファ値
	static bool IsFade();
	static void FadeIn();
	static void FadeOut();
	void LateUpdate();
	void Draw() override;//オーバーライドを付けると元の名前が変わるとエラーが出る
};

class Timer : public Task {
private:
	CImage m_img;
	CImage m_img2;
public:
	Timer();
	~Timer();
	int m_ang;
	int m_ang2;
	int m_time;
	void Update() override;
	void Draw() override;
};
