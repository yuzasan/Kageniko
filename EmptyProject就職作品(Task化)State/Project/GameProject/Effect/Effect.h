#pragma once
#include "../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class Effect :public Task {
private:
	CModelObj	m_model;		//モデル
	CVector3D	m_vec_accel;	//移動量増加
	CVector3D	m_rot_vec;		//回転量
	int			m_blend;		//ブレンド種類
	bool		m_builbord;		//ビルボード描画設定
	float		m_scale;		//スケール
	float		m_scale_speed;	//スケール変化量
	float		m_alpha;		//アルファ値
	float		m_alpha_speed;	//アルファ値変化量
	bool		m_depth;		//深度バッファ設定
	int m_time;
public:
	enum {
		eBlendAlpha,	//通常ブレンド
		eBlendAdd,		//加算ブレンド
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