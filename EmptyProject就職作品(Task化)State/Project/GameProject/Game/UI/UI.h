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
	bool m_isFadeOut;	//�t�F�[�h�A�E�gor
	bool m_isFade;		//�t�F�[�h��
	float m_alpha;			//�A���t�@�l
	static bool IsFade();
	static void FadeIn();
	static void FadeOut();
	void LateUpdate();
	void Render() override;//�I�[�o�[���C�h��t����ƌ��̖��O���ς��ƃG���[���o��
};
