#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class PlayerCamera;

class Player : public CharaBase {
private:
	//���f���I�u�W�F�N�g
	CModelA3M m_model;
	CVector3D m_rot_target;
	//Task* mp_camera;
	PlayerCamera* mp_camera;
	Task* mp_enemy;
	Task* mp_isenemy;
	Task* mp_tyukan;
	Task* mp_fellbox;
	//�ړ����x
	//const float move_speed = 0.1f;
	const float move_speed = 0.2f;
	//const float move_speed = 0.3f;
	//�F
	int m_gauge;
	//int cnt = 0;
	//���ԍ��W
	CVector3D m_tyukan;
public:
	//bool m_isFind;
	Player(const CVector3D& pos);
	~Player();
	void Update();
	void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	void NoEnemyRender();
	void Shot();
	//�f�o�b�O����
	void Debug();
	void Collision(Task* b);
	void LateUpdate();
};