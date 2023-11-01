#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class PlayerCamera;

class Player : public CharaBase {
private:
	//モデルオブジェクト
	CModelA3M m_model;
	CVector3D m_rot_target;
	//Task* mp_camera;
	PlayerCamera* mp_camera;
	Task* mp_enemy;
	Task* mp_isenemy;
	Task* mp_tyukan;
	Task* mp_fellbox;
	//移動速度
	//const float move_speed = 0.1f;
	const float move_speed = 0.2f;
	//const float move_speed = 0.3f;
	//色
	int m_gauge;
	//int cnt = 0;
	//中間座標
	CVector3D m_tyukan;
public:
	//bool m_isFind;
	Player(const CVector3D& pos);
	~Player();
	void Update();
	void Render();
	/// <summary>
	/// enemy以外の描画処理
	/// </summary>
	void NoEnemyRender();
	void Shot();
	//デバッグ処理
	void Debug();
	void Collision(Task* b);
	void LateUpdate();
};