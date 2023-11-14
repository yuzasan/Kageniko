#pragma once
#include "../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class Effect :public Task {
private:
	CModelObj	m_model;		//���f��
	CVector3D	m_vec_accel;	//�ړ��ʑ���
	CVector3D	m_rot_vec;		//��]��
	int			m_blend;		//�u�����h���
	bool		m_builbord;		//�r���{�[�h�`��ݒ�
	float		m_scale;		//�X�P�[��
	float		m_scale_speed;	//�X�P�[���ω���
	float		m_alpha;		//�A���t�@�l
	float		m_alpha_speed;	//�A���t�@�l�ω���
	bool		m_depth;		//�[�x�o�b�t�@�ݒ�
	int m_time;
public:
	enum {
		eBlendAlpha,	//�ʏ�u�����h
		eBlendAdd,		//���Z�u�����h
	};
	Effect(const char* model, const CVector3D& pos, const CVector3D& vec, const CVector3D& accel, float scale, float change_scale, float alpha, float change_alpha, int blend, bool builbord, bool depth, int time = -1);
	~Effect();
	void Update();
	void Render();
};

class Effect2D :public Task {
private:
	CImage m_img;
	Player* mp_player;
public:
	Effect2D(const char* img);
	~Effect2D();
	void Update();
	void LateRender();
};