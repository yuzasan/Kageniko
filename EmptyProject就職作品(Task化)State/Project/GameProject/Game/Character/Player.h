#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class PlayerCamera;

class Player : public CharaBase {
private:
	CModelA3M m_model;		//���f���f�[�^
	CVector3D m_moveDir;	//�ړ�����
	CVector3D m_rot_target;
	//Task* mp_camera;
	PlayerCamera* mp_camera;
	Task* mp_enemy;
	Task* mp_isenemy;
	Task* mp_tyukan;
	Task* mp_fellbox;
	//�F
	int m_gauge;
	CVector3D m_tyukan;		//���ԍ��W
	float m_elapsedTime;	//�ҋ@�o�ߎ���

	//�A�j���[�V�����ԍ�
	enum class AnimId 
	{
		Idle,	//�ҋ@
		Walk,	//����
	};

	//���
	enum class State
	{
		Move,		//�ړ����
		Invisible,	//�������	
	};
	State m_state;	//���݂̏��

	//�ړ���Ԃ̏���
	void StateMove();
	//������Ԃ̏���
	void StateInvisible();

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